using System.Text.Json;

namespace Preprocessor;

static class Program
{
    private static Dictionary<string, DateTime> _cachedLastWriteTimes = new();

    static void Main(string[] args)
    {
        Console.ForegroundColor = ConsoleColor.White;
        Console.WriteLine("We're running the preprocessor!");

        LoadLastWriteTimes();

        if (args.Length < 3)
        {
            Console.WriteLine("Please provide a directory to search for header files, and the build type.");
            return;
        }

        var dir = args[0];
        var projName = args[1];
        Console.WriteLine(dir);
        dir = Path.GetFullPath(dir);

        Console.WriteLine("Performing preprocessor copies");
        DoPreprocessorCopies(dir, projName, args[2]);

        Console.WriteLine($"Looking for header files in {dir}/{projName}/Include/"); // Remove the 
        var files = Directory.GetFiles($"{dir}/{projName}/Include/", "*.h", SearchOption.AllDirectories);
        foreach (var file in files)
        {
            var lastWriteTime = File.GetLastWriteTime(file);
            if (_cachedLastWriteTimes.TryGetValue(file, out var cachedLastWriteTime) && !args.Contains("--force"))
            {
                if (lastWriteTime == cachedLastWriteTime)
                    continue;
            }

            ProcessHeader(file);
        }

        SaveLastWriteTimes();
    }

    static void DoPreprocessorCopies(string dir, string projName, string buildType)
    {
        if (!File.Exists($"{dir}/{projName}/PreprocessorCopies.txt"))
            return;

        string platform = "Unknown";
        if (buildType.Contains("windows"))
            platform = "Win64";
        else if (buildType.Contains("linux"))
            platform = "Linux64";

        var lines = File.ReadAllLines($"{dir}/{projName}/PreprocessorCopies.txt");
        foreach (var line in lines)
        {
            if (string.IsNullOrWhiteSpace(line)) continue;

            var parts = line.Split(" ");
            int mode = 0;
            if (parts[0] == "always")
                mode = 0;
            else if (parts[0] == "ifnewer")
                mode = 1;
            else
            {
                Console.WriteLine($"Unknown mode {parts[0]} in PreprocessorCopies.txt");
                continue;
            }

            bool shouldCopy = true;
            foreach (var part in parts)
            {
                if (part.StartsWith("plat="))
                    if (platform != part.Split('=')[1])
                        shouldCopy = false;
            }

            if (!shouldCopy) continue;

            var source = parts[1];
            var dest = parts[2];

            FixUpDirectorySeparator(ref source);
            FixUpDirectorySeparator(ref dest);

            void CopyFile(string src, string destination)
            {
                if (!File.Exists(src))
                {
                    Console.WriteLine($"Source file {src} doesn't exist!");
                    return;
                }

                if (mode == 1
                    && File.Exists(destination))
                {
                    var sourceTime = File.GetLastWriteTime(src);
                    var destTime = File.GetLastWriteTime(destination);
                    if (sourceTime <= destTime)
                        return;
                }

                var newDir = Path.GetDirectoryName(destination)!;
                if (!Directory.Exists(newDir))
                    Directory.CreateDirectory(newDir);

                File.Copy(src, destination, true);
            }

            source = Path.Combine(dir, source);
            source = source.Replace("{Lib}", $"/Lib/{platform}/");
            dest = Path.Combine(dir, dest);
            dest = dest.Replace("{BuildPath}", $"/Build/{projName}/{buildType}/");
            FixUpDirectorySeparator(ref dest);

            if (File.Exists(source))
                CopyFile(source, Path.Combine(dest, Path.GetFileName(source)));
            else if (Directory.Exists(source))
            {
                // Copy the directory
                var files = Directory.GetFiles(source, "*", SearchOption.AllDirectories);
                foreach (var file in files)
                {
                    var relativePath = file.Substring(source.Length);
                    var destPath = Path.Combine(dest, relativePath);
                    CopyFile(file, destPath);
                }
            }
            else
            {
                Console.WriteLine($"Source {source} doesn't exist - not a file, nor a directory!");
            }
        }
    }

    static void LoadLastWriteTimes()
    {
        if (!File.Exists("lastWriteTimes.json"))
            return;
        var json = File.ReadAllText("lastWriteTimes.json");
        _cachedLastWriteTimes = JsonSerializer.Deserialize<Dictionary<string, DateTime>>(json)!;

        if (_cachedLastWriteTimes is null)
        {
            Console.WriteLine("Failed to load last write times, starting fresh!");
            _cachedLastWriteTimes = new();
        }
    }

    static void SaveLastWriteTimes()
    {
        var json = JsonSerializer.Serialize(_cachedLastWriteTimes);
        File.WriteAllText("lastWriteTimes.json", json);
    }

    static void ProcessHeader(string path)
    {
        FixUpDirectorySeparator(ref path);

        if (_cachedLastWriteTimes.ContainsKey(path))
            _cachedLastWriteTimes[path] = File.GetLastWriteTime(path);
        else
            _cachedLastWriteTimes.Add(path, File.GetLastWriteTime(path));

        if (path.ToLower().Contains("vendor")) // Lets not process vendor headers!
            return;

        Console.WriteLine($"Processing {path}");

        HeaderProcessor processor = new(File.ReadAllText(path));
        var entities = processor.Process();
        if (entities.Count == 0)
            return;
        Console.WriteLine($"Found {entities.Count} entities in {path}:");
        foreach (var entity in entities) Console.WriteLine($"    {entity.Name}");
        GenerateSourceForFile(path, entities);
    }

    static void GenerateSourceForFile(string path, List<Entity> entities)
    {
        var sourcePath = path.Replace("Include", "Source").Replace(".h", ".cpp");
        if (!Path.Exists(sourcePath))
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine($"Couldn't find source file at {sourcePath}");
            Console.ForegroundColor = ConsoleColor.White;
            _cachedLastWriteTimes.Remove(path);
            return;
        }

        Console.WriteLine($"Writing reflection code to {sourcePath}");
    }

    static void FixUpDirectorySeparator(ref string path)
    {
        path = path.Replace('/', Path.DirectorySeparatorChar).Replace('\\', Path.DirectorySeparatorChar);
    }
}

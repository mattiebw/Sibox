namespace Preprocessor;

public class Func
{
    public Func(string content)
    {
    }
}

public class Property
{
    public Property(string content)
    {
    }
}

public enum RPCType
{
    Multicast,
    Client,
    Server
}

public class RPC
{
    public RPCType Type;
    
    public RPC(string content)
    {
        var parts = content.Split(',');
        for (var i = 0; i < parts.Length; i++)
            parts[i] = parts[i].Trim();
        
        Type = parts[0].ToLower() switch
        {
            "multicast" => RPCType.Multicast,
            "client"    => RPCType.Client,
            "server"    => RPCType.Server,
            _           => throw new Exception($"Unknown RPC type: {parts[0]}")
        };  
    }
}

public class Entity(string name)
{
    public String Name = name;
    public List<Func> Functions = new();
    public List<Property> Properties = new();
    public List<RPC> RPCs = new();
}

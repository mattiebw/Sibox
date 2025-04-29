#pragma once

class FileUtil
{
public:
	static Buffer ReadBinaryFileToBuffer(const std::filesystem::path &path)
	{
		if (!exists(path))
		{
			SIBOX_ERROR("Tried to read file that doesn't exist: {}", path.string());
			return Buffer(); // Return empty buffer
		}

		std::ifstream file(path, std::ios::binary);
		if (!file.is_open())
		{
			SIBOX_ERROR("Failed to open file: {}", path.string());
			return Buffer(); // Return empty buffer
		}

		// Seek to end. This works better than ios::ate, as it properly reports the size in bytes in all circumstances.
		file.ignore(std::numeric_limits<std::streamsize>::max());
		const std::streamsize pos = file.gcount();
		Buffer                buf(pos);
		file.seekg(0, std::ios::beg);
		file.read(reinterpret_cast<char*>(buf.Data), pos);
		file.close();

		return buf;
	}
};

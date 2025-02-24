#pragma once

class SavedDataManager
{
public:
	void Init(const std::filesystem::path &rootPath);

	bool   FileExists(const std::filesystem::path &path);
	Buffer GetBinaryData(const std::filesystem::path &path);
	bool   SaveBinaryData(const std::filesystem::path &path, const Buffer &buffer);
	bool   Delete(const std::filesystem::path &path);

protected:
	std::filesystem::path m_RootPath;
};

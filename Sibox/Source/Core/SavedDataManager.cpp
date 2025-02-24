#include "papipch.h"
#include "Core/SavedDataManager.h"

void SavedDataManager::Init(const std::filesystem::path &rootPath)
{
	m_RootPath = rootPath;
	create_directories(m_RootPath);
}

bool SavedDataManager::FileExists(const std::filesystem::path &path)
{
	auto          absPath = m_RootPath / path;
	std::ifstream infile(absPath);
	return infile.good();
}

Buffer SavedDataManager::GetBinaryData(const std::filesystem::path &path)
{
	// Open the file
	std::filesystem::path absPath = m_RootPath / path;
	std::ifstream         infile(absPath, std::ios::binary | std::ios::ate);
	if (!infile.good())
	{
		PAPI_ERROR("Failed to open and read file {0}", absPath.string());
		return {nullptr, 0};
	}

	// Read in all the data
	std::streamsize size = infile.tellg();
	infile.seekg(0, std::ios::beg);
	char *buffer = new char[size];
	infile.read(buffer, size);
	infile.close();

	// Return as buffer. We leave deleting the data to the caller.
	return {reinterpret_cast<uint8_t*>(buffer), static_cast<size_t>(size)};
}

bool SavedDataManager::SaveBinaryData(const std::filesystem::path &path, const Buffer &buffer)
{
	std::filesystem::path absPath = m_RootPath / path;
	create_directories(absPath.parent_path());
	std::ofstream outfile(absPath, std::ios::binary);
	if (!outfile.good())
	{
		PAPI_ERROR("Failed to open and write data to file {0}", absPath.string());
		return false;
	}

	outfile.write(reinterpret_cast<char*>(buffer.Data), buffer.Size);

	return true;
}

bool SavedDataManager::Delete(const std::filesystem::path &path)
{
	return std::filesystem::remove(path) == 0;
}

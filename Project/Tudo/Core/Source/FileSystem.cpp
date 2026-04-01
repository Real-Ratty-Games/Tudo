/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "FileSystem.hpp"
#if !TUDO_NO_LOGGER
#include "Logger.hpp"
#endif
#include <filesystem>
#include <fstream>

#if __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

using namespace Tudo;

bool FileSystem::Exists(strgv filepath)
{
	return std::filesystem::exists(std::filesystem::path(filepath));
}

void FileSystem::FileRemove(strgv filepath)
{
	strg fp(filepath);
	std::filesystem::remove(fp);
#if !TUDO_NO_LOGGER
	Logger::Log("File removed '" + fp + "'");
#endif
}

std::vector<strg> FileSystem::FilesInDirectory(strgv dir, int nt)
{
	if (Exists(dir))
	{
		std::vector<strg> rtVl;
		for (const auto& entry : std::filesystem::directory_iterator(dir))
		{
			if (entry.is_regular_file())
			{
				strg pb = entry.path().string();
				switch (nt)
				{
				case 1: { pb = entry.path().stem().string(); }break;
				case 2: { pb = entry.path().extension().string(); }break;
				}
				rtVl.push_back(pb);
			}
		}
		return rtVl;
	}
	return {};
}

std::vector<strg> FileSystem::SubDirectories(strgv dir)
{
	if (Exists(dir))
	{
		std::vector<strg> rtVl;
		for (const auto& entry : std::filesystem::directory_iterator(dir))
		{
			if (entry.is_directory())
			{
				strg pb = entry.path().string();
				rtVl.push_back(pb);
			}
		}
		return rtVl;
	}
	return {};
}

void FileSystem::DirectoryCreate(strgv dir)
{
	strg dr(dir);
	std::filesystem::create_directory(dr);
#if !TUDO_NO_LOGGER
	Logger::Log("Directory created '" + dr + "'");
#endif
}

void FileSystem::DirectoryRemove(strgv dir)
{
	strg dr(dir);
	std::filesystem::remove_all(dr);
#if !TUDO_NO_LOGGER
	Logger::Log("Directory removed '" + dr + "'");
#endif
}

std::vector<char> FileSystem::ReadBinaryFile(strgv filepath)
{
	std::vector<char> rtVl;

	std::ifstream File(filepath.data(), std::ios::in | std::ios::binary | std::ios::ate);
	std::streamsize size = File.tellg();
	rtVl.resize(size);

	File.seekg(0, std::ios::beg);
	File.read(rtVl.data(), size);
	File.close();

	return rtVl;
}

void FileSystem::WriteBinaryFile(strgv filepath, const std::vector<char>& data)
{
	strg fp(filepath);
	std::ofstream file(fp, std::ios::binary);
	file.write(data.data(), data.size());
	file.close();
#if !TUDO_NO_LOGGER
	Logger::Log("Binary file written '" + fp + "'");
#endif
}

void FileSystem::WriteTextFile(strgv filepath, strgv text)
{
	strg fp(filepath);
	std::ofstream file(fp);
	file.write(text.data(), text.size());
	file.close();
#if !TUDO_NO_LOGGER
	Logger::Log("Text file written '" + fp + "'");
#endif
}

std::filesystem::path FileSystem::GetResourcePath(strgv filename)
{
#if __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
        char path[PATH_MAX];
        if (CFURLGetFileSystemRepresentation(resourcesURL, true, (uint8*)path, PATH_MAX)) {
            CFRelease(resourcesURL);
            return std::filesystem::path(path) / filename;
        }
        CFRelease(resourcesURL);
#endif
    return filename;
}

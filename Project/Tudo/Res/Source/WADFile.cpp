/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "WADFile.hpp"
#include "Logger.hpp"
#include "FileSystem.hpp"
#include <fstream>
#include <sstream>

using namespace Tudo;

bool WADFile::Open(strgv filepath)
{
	if (!FileSystem::Exists(filepath))
	{
		Logger::Log("WADFile::Open", "Cannot open WAD file, file does not exist: " + strg(filepath), ELogType::LWARNING);
		return false;
	}
	mFilepath = filepath;

	std::ifstream file(mFilepath.data(), std::ios::binary | std::ios::ate);
	file.seekg(0, std::ios::beg);

	uint fileCnt = 0;
	file.read(reinterpret_cast<char*>(&fileCnt), sizeof(fileCnt));

	for (uint i = 0; i < fileCnt; i++)
	{
		uint16 nameLength = 0;
		file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

		strg fileName(nameLength, '\0');
		file.read(&fileName[0], nameLength);

		uint fileSize, fileOffset;
		uint8 fileType;
		file.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
		file.read(reinterpret_cast<char*>(&fileOffset), sizeof(fileOffset));
		file.read(reinterpret_cast<char*>(&fileType), sizeof(fileType));

		WADItem item(fileSize, fileOffset, fileType);
		mItems.insert(std::make_pair(fileName, item));
	}
	return true;
}

bool WADFile::Read(strgv itemname, std::vector<uint8>& data)
{
	const strg iname(itemname);
	if (!mItems.contains(iname))
	{
		Logger::Log("WADFile::Read", "Item " + iname + " could not be found inside: " + mFilepath, ELogType::LWARNING);
		return false;
	}

	WADItem& item = mItems[iname];

	std::ifstream file(mFilepath.data(), std::ios::binary);
	if (!file)
	{
		Logger::Log("WADFile::Read", "Cannot open WAD file: " + mFilepath, ELogType::LWARNING);
		return false;
	}

	data.resize(item.Size);

	file.seekg(static_cast<std::streampos>(item.Offset), std::ios::beg);
	if (!file.read(reinterpret_cast<char*>(data.data()), item.Size))
	{
		Logger::Log("WADFile::Read", "Failed reading data here!", ELogType::LWARNING);
		return false;
	}
	return true;
}

std::unordered_map<strg, WADItem>& WADFile::Items()
{
	return mItems;
}

void WADFile::Clear()
{
	mItems.clear();
	mFilepath.clear();
}

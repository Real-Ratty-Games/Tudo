/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Logger.hpp"
#include "BigError.hpp"
#include "FileSystem.hpp"

using namespace Tudo;

std::vector<LogEntry> Logger::mEntries;

void Logger::Log(strgv message, ELogType type)
{
	strg msg(message);
	mEntries.push_back({ msg, type });

	if (type == ELogType::LERROR)
	{
		Save("CrashDump.log");
		throw BigError(msg);
	}
}

void Logger::Log(strgv src, strgv message, ELogType type)
{
	strg msg = strg(src) + " -> " + strg(message);
	Log(msg, type);
}

uint64 Logger::Count(ELogType type)
{
	if (type == ELogType::LCOUNT)
		return mEntries.size();

	return std::count_if(mEntries.begin(), mEntries.end(),
		[&](const LogEntry& e)
		{
			return (e.Type == type);
		});
}

void Logger::Save(strgv filename, ELogType type)
{
	strg out =	"Messages: " + std::to_string(Count());
	out += "\nInfos:    " + std::to_string(Count(ELogType::LINFO));
	out += "\nWarnings: " + std::to_string(Count(ELogType::LWARNING));
	out += "\nErrors:   " + std::to_string(Count(ELogType::LERROR)) + "\n\n";

	for (auto& it : mEntries)
	{
		switch (it.Type)
		{
		case ELogType::LINFO:
		{
			out += "Info:    ";
		}break;
		case ELogType::LWARNING:
		{
			out += "Warning: ";
		}break;
		case ELogType::LERROR:
		{
			out += "Error:   ";
		}break;
		}
		out += it.Message + "\n";
	}

	FileSystem::WriteTextFile(filename, out);
}

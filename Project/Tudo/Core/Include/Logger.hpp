/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef LOGGER_HPP_
#define LOGGER_HPP_
#include "SystemTypes.hpp"
#include <vector>

namespace Tudo
{
	enum class ELogType
	{
		LINFO,
		LWARNING,
		LERROR,

		LCOUNT		// don't use
	};

	struct LogEntry
	{
		strg		Message;
		ELogType	Type;
	};

	class Logger
	{
	public:
		static void		Log(strgv message, ELogType type = ELogType::LINFO);
		static void		Log(strgv src, strgv message, ELogType type = ELogType::LINFO);
		static uint64	Count(ELogType type = ELogType::LCOUNT);
		static void		Save(strgv filename, ELogType type = ELogType::LCOUNT);

	private:
		static std::vector<LogEntry> mEntries;
	};
}
#endif
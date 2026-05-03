/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Memory.hpp"

using namespace Tudo;

void Memory::Copy(void* dst, const void* src, uint64 size)
{
	if (!dst || !src || size == 0)
		Logger::Log("Memory::Copy", "Memory is invalid!", ELogType::LERROR);
	std::memcpy(dst, src, size);
}

/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef MEMORY_HPP_
#define MEMORY_HPP_
#include "SystemTypes.hpp"
#include "Logger.hpp"
#include <utility>

#define TUDO_MEM_FREE(p) { delete p; p = nullptr; }

namespace Tudo
{
	namespace Memory
	{
		void Copy(void* dst, const void* src, uint64 size);

		template<typename T, uint64 N>
		uint64 ArrayCount(T(&arr)[N]) noexcept
		{
			return N;
		}
	}
}
#endif
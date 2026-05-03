/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Engine.hpp"
#include <DebugHelp.hpp>
#include <Logger.hpp>

using namespace Tudo;

int main()
{
#if _DEBUG
	DEBUGHELP_LOOK4MEMLEAKS();
#endif
	DEBUGHELP_SHOWCMD(false);

	{
		Engine engine;
		engine.Run();
	}
	Logger::Save("EndDump.log");
	return 0;
}

/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "../Include/Game.hpp"
#include <DebugHelp.hpp>
#include <Logger.hpp>

using namespace MyGame;

void RunProgram();

int main()
{
#if _DEBUG
	DEBUGHELP_LOOK4MEMLEAKS();
#endif
	DEBUGHELP_SHOWCMD(false);

	RunProgram();
	Logger::Save("EndDump.log");
	return 0;
}

void RunProgram()
{
	GameProgram program;
	program.Run();
}

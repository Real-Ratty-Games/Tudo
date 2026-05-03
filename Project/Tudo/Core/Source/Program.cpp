/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Program.hpp"
#include "Globals.hpp"
#include "Logger.hpp"
#include <format>
#include <chrono>

using namespace Tudo;

Program::Program() : bQuit(false) {}

void Program::Run()
{
	Startup();

	if (Initialize())
	{
		Logger::Log("Program successfully initialized!");
		while (!bQuit)
		{
			Tick();
			Draw();
		}
	}
	Cleanup();
	Logger::Log("Program Cleanup called!");
}

void Program::Startup()
{
	auto now = std::chrono::system_clock::now();	// getting local time causes mem leaks in std...

	Logger::Log("Program Startup");
	Logger::Log("Today is " + std::format("{:%d-%m-%Y}", now));
	Logger::Log("Tudo Engine Version " + std::format("{:.2f}", TUDO_VERSION / 100.0));

	strg sys = "Windows x64";
#if __APPLE__
	sys = "macOS arm64";
#endif
	Logger::Log("System is " + sys);

	strg mod = "Release";
#if _DEBUG
	mod = "Debug";
#endif
	Logger::Log("Build type is " + mod);
}

void Program::Quit(bool vl)
{
	bQuit = vl;
	Logger::Log("Program quit requested!");
}

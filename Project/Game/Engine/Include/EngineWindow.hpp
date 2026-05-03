/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef ENGINEWINDOW_HPP_
#define ENGINEWINDOW_HPP_
#include <SystemTypes.hpp>
#include <Window.hpp>
#include <Program.hpp>

using namespace Tudo;

class EngineWindow : public Window
{
public:
	EngineWindow(Program* program);

protected:
	void EventCallback();

private:
	Program* pProgram;
};

#endif
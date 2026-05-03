/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_
#include "SystemTypes.hpp"

namespace Tudo
{
	class Program
	{
	public:
		Program();
		void Run();
		void Quit(bool vl = true);

	private:
		void Startup();

	protected:
		virtual bool Initialize() = 0;
		virtual void Tick() = 0;
		virtual void Draw() = 0;
		virtual void Cleanup() = 0;

	private:
		bool bQuit;
	};
}
#endif
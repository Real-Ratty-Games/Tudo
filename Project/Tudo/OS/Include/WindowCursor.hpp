/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef WINDOWCURSOR_HPP_
#define WINDOWCURSOR_HPP_
#include "SystemTypes.hpp"
#include "Window.hpp"

namespace Tudo
{
	class WindowCursor
	{
	public:
		/// format must be bmp
		void Load(strgv imagePath);
		~WindowCursor();

		friend class Window;

	private:
		SDL_Cursor* mCursor;
	};
}
#endif
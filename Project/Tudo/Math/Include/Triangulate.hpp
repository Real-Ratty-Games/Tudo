/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef TRIANGULATE_HPP_
#define TRIANGULATE_HPP_
#include "SystemTypes.hpp"
#include <vector>

namespace Tudo::Math
{
	void	Triangulate2D(const std::vector<vec2>& points, std::vector<int>& indices);
	float	SignedArea(const std::vector<vec2>& points);
}
#endif
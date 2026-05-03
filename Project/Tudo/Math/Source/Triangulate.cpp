/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Triangulate.hpp"

using namespace Tudo;

void Math::Triangulate2D(const std::vector<vec2>& points, std::vector<int>& indices)
{
	std::vector<int> v(points.size());

	for (int i = 0; i < points.size(); i++) v[i] = i;

	while (v.size() > 3)
	{
		bool earFound = false;

		for (uint64 i = 0; i < v.size(); i++)
		{
			int i0 = v[(i + v.size() - 1) % v.size()];
			int i1 = v[i];
			int i2 = v[(i + 1) % v.size()];

			if (vec2::Cross(points[i0], points[i1], points[i2]) <= 0)
				continue;

			bool contains = false;
			for (uint64 j = 0; j < v.size(); j++)
			{
				int vi = v[j];
				if (vi == i0 || vi == i1 || vi == i2)
					continue;

				float d1 = vec2::Sign(points[vi], points[i0], points[i1]);
				float d2 = vec2::Sign(points[vi], points[i1], points[i2]);
				float d3 = vec2::Sign(points[vi], points[i2], points[i0]);

				if ((d1 >= 0 && d2 >= 0 && d3 >= 0))
				{
					contains = true;
					break;
				}
			}

			if (!contains)
			{
				indices.push_back(i0);
				indices.push_back(i1);
				indices.push_back(i2);

				v.erase(v.begin() + i);
				earFound = true;
				break;
			}
		}
		if (!earFound) break;
	}

	if (v.size() == 3)
	{
		indices.push_back(v[0]);
		indices.push_back(v[1]);
		indices.push_back(v[2]);
	}
}

float Math::SignedArea(const std::vector<vec2>& points)
{
	float a = 0.0f;
	for (uint64 i = 0; i < points.size(); i++)
	{
		const vec2& p0 = points[i];
		const vec2& p1 = points[(i + 1) % points.size()];
		a += (p0.X * p1.Y - p1.X * p0.Y);
	}
	return a * 0.5f;
}

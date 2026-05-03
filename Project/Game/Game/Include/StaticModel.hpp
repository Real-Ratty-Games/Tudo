/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef STATICMODEL_HPP_
#define STATICMODEL_HPP_
#include <SystemTypes.hpp>
#include "Actor.hpp"

using namespace Tudo;

class StaticModel : public Actor
{
public:
	StaticModel();
	void Begin();
	void End();

public:
	vec3 Scale;
	quat Rotation;
};
#endif
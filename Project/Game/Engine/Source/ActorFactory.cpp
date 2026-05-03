/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "ActorFactory.hpp"

Actor* ActorFactory::Create(const strg& name)
{
	return mCreators[name]();
}

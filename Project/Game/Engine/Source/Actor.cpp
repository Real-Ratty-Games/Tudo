/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Actor.hpp"

Actor::Actor() :
	bTick(false),
	bPreTick(false),
	Location(0)
{
	InitProperty(Location, "Location");
}

void Actor::Begin() {}
void Actor::End() {}
void Actor::PreTick() {}
void Actor::Tick() {}

uint64 Actor::ID()
{
	return mID;
}

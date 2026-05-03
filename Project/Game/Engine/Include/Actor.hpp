/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef ACTOR_HPP_
#define ACTOR_HPP_
#include <SystemTypes.hpp>
#include <unordered_map>

using namespace Tudo;

class World;

struct ActorProperty
{
	uint64	Size = 0;
	void*	Data = nullptr;
};

class Actor
{
public:
	Actor();
	virtual ~Actor() = default;

	virtual void Begin();
	virtual void End();
	virtual void PreTick();
	virtual void Tick();

	uint64 ID();

	template<typename T>
	void InitProperty(T& property, const strg& name)
	{
		if(!mProperties.contains(name))
			mProperties[name] = { sizeof(T), &property };
	}

	friend class World;

public:
	vec3 Location;

protected:
	bool bTick;
	bool bPreTick;

	World* pWorld;

private:
	uint64	mID;
	strg	mClassName;
	std::unordered_map<strg, ActorProperty> mProperties;
};
#endif
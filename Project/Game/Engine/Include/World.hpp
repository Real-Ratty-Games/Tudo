/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef WORLD_HPP_
#define WORLD_HPP_
#include <SystemTypes.hpp>
#include <vector>
#include "Actor.hpp"
#include "ActorFactory.hpp"

using namespace Tudo;

#define WORLD_MAGIC 0x236B1

class World
{
public:
	World();
	~World();
	void PreTick();
	void Tick();

	void Load(const strg& filename);
	void Save(const strg& filename);

	Actor*	PlaceActor(const strg& classname);
	void	KillActor(Actor* actor);
	Actor*	FindActor(uint64 id);
	void	KillActors();

	ActorFactory& GetActorFactory();

private:
	void SortActors();
	void KillActiveActors();
	void BeginActors();

public:
	bool bPaused;

private:
	bool				bSort;
	bool				bKill;
	bool				bBegin;

	uint64				mActorCnt;
	std::vector<Actor*> mActors;
	std::vector<Actor*> mActorsTick;
	std::vector<Actor*> mActorsPreTick;
	std::vector<Actor*> mActorsKill;
	std::vector<Actor*> mActorsBegin;

	ActorFactory mFactory;
};
#endif
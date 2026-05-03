/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "World.hpp"
#include <Logger.hpp>
#include <Memory.hpp>
#include <fstream>

World::World() :
	bPaused(false),
	bSort(false),
	bKill(false),
	mActorCnt(0)
{}

World::~World()
{
	KillActors();
}

void World::PreTick()
{
	if (bBegin) BeginActors();
	if (bKill) KillActiveActors();
	if (bSort) SortActors();

	for (auto& it : mActorsPreTick)
		it->PreTick();
}

void World::Tick()
{
	for (auto& it : mActorsTick)
		it->Tick();
}

void World::Load(const strg& filename)
{
	KillActors();

	std::ifstream file(filename.data(), std::ios::binary);
	if (!file)
	{
		Logger::Log("World::Load", "Failed loading world '" + filename + "'!", ELogType::LWARNING);
		return;
	}

	file.seekg(0, std::ios::beg);

	int magicn;
	file.read(reinterpret_cast<char*>(&magicn), sizeof(int));

	if(magicn != WORLD_MAGIC)
	{
		Logger::Log("World::Load", "Failed loading world '" + filename + "', invalid format!", ELogType::LWARNING);
		return;
	}

	uint64 actorcnt;
	file.read(reinterpret_cast<char*>(&actorcnt), sizeof(uint64));

	for (uint64 i = 0; i < actorcnt; i++)
	{
		uint64 cnamesize;
		file.read(reinterpret_cast<char*>(&cnamesize), sizeof(uint64));

		char* cname = new char[cnamesize + 1];
		file.read(cname, cnamesize);
		cname[cnamesize] = '\0';
		strg scname(cname);
		delete[] cname;

		Actor* actor = PlaceActor(scname);
		if (actor)
		{
			uint64 rsize = actor->mProperties.size();

			uint64 size;
			file.read(reinterpret_cast<char*>(&size), sizeof(uint64));

			if (rsize != size)
				Logger::Log("World::Load", "Properties of '" + scname + "' have changed!", ELogType::LWARNING);

			for (uint64 pi = 0; pi < size; pi++)
			{
				uint64 pnamesize;
				file.read(reinterpret_cast<char*>(&pnamesize), sizeof(uint64));

				char* pname = new char[pnamesize + 1];
				file.read(pname, pnamesize);
				pname[pnamesize] = '\0';
				strg pscname(pname);
				delete[] pname;

				uint64 psize;
				file.read(reinterpret_cast<char*>(&psize), sizeof(uint64));

				if (actor->mProperties.contains(pscname))
				{
					auto& p = actor->mProperties[pscname];
					file.read(reinterpret_cast<char*>(p.Data), psize);
				}
				else file.ignore(psize);
			}
		}
		else
		{
			uint64 size;
			file.read(reinterpret_cast<char*>(&size), sizeof(uint64));

			for (uint64 pi = 0; pi < size; pi++)
			{
				uint64 pnamesize;
				file.read(reinterpret_cast<char*>(&pnamesize), sizeof(uint64));
				file.ignore(pnamesize);

				uint64 psize;
				file.read(reinterpret_cast<char*>(&psize), sizeof(uint64));
				file.ignore(psize);
			}
		}
	}

	Logger::Log("World::Load", "World '" + filename + "' loaded successfully!");
}

void World::Save(const strg& filename)
{
	std::ofstream out(filename.data(), std::ios::binary | std::ios::trunc);
	if (!out)
	{
		Logger::Log("World::Save", "Failed saving world '" + filename + "'!", ELogType::LWARNING);
		return;
	}

	int magicn = WORLD_MAGIC;
	uint64 actorcnt = mActors.size();

	out.write(reinterpret_cast<const char*>(&magicn), sizeof(int));
	out.write(reinterpret_cast<const char*>(&actorcnt), sizeof(uint64));

	for (auto& it : mActors)
	{
		uint64 cnamesize = it->mClassName.size();
		out.write(reinterpret_cast<const char*>(&cnamesize), sizeof(uint64));
		out.write(it->mClassName.data(), cnamesize);

		uint64 size = it->mProperties.size();
		out.write(reinterpret_cast<const char*>(&size), sizeof(uint64));

		for (auto& p : it->mProperties)
		{
			strg pname = p.first;
			ActorProperty& prp = p.second;

			uint64 pnamesize = pname.size();
			out.write(reinterpret_cast<const char*>(&pnamesize), sizeof(uint64));
			out.write(pname.data(), pnamesize);

			out.write(reinterpret_cast<const char*>(&prp.Size), sizeof(uint64));
			out.write(reinterpret_cast<const char*>(prp.Data), prp.Size);
		}
	}

	if(!out.good())
		Logger::Log("World::Save", "Failed saving world '" + filename + "'!", ELogType::LWARNING);

	Logger::Log("World::Save", "World saved as '" + filename + "'!");
}

Actor* World::PlaceActor(const strg& classname)
{
	if (!mFactory.mCreators.contains(classname))
	{
		Logger::Log("World::PlaceActor", "Failed creating actor '" + classname + "', class not registered!", ELogType::LWARNING);
		return nullptr;
	}

	Actor* actor = mFactory.Create(classname);

	actor->pWorld = this;
	actor->mClassName = classname;
	actor->mID = mActorCnt;
	mActors.push_back(actor);

	mActorsBegin.push_back(actor);

	bBegin = true;
	bSort = true;
	mActorCnt++;
	return actor;
}

void World::KillActor(Actor* actor)
{
	bKill = true;

	auto it = std::find(mActors.begin(), mActors.end(), actor);
	auto it2 = std::find(mActorsKill.begin(), mActorsKill.end(), actor);
	
	if (it != mActors.end() && it2 == mActorsKill.end())
		mActorsKill.push_back(actor);
}

Actor* World::FindActor(uint64 id)
{
	auto it = std::find_if(mActors.begin(), mActors.end(), [&](const Actor* actor) {
		return actor->mID == id;
		});

	if (it != mActors.end())
		return *it;
	return nullptr;
}

void World::KillActors()
{
	for (auto& it : mActors)
	{
		it->End();
		TUDO_MEM_FREE(it);
	}
	mActors.clear();
	mActorsTick.clear();
	mActorsPreTick.clear();
	mActorsKill.clear();
	mActorCnt = 0;
	bSort = false;
	bKill = false;
}

void World::SortActors()
{
	bSort = false;

	mActorsTick.clear();
	mActorsPreTick.clear();

	for (auto& it : mActors)
	{
		if (it->bPreTick) mActorsPreTick.push_back(it);
		if (it->bTick) mActorsTick.push_back(it);
	}
}

void World::KillActiveActors()
{
	bSort = true;
	bKill = false;

	mActors.erase(
		std::remove_if(mActors.begin(), mActors.end(),
			[&](const Actor* value) {
				return std::find(
					mActorsKill.begin(),
					mActorsKill.end(),
					value
				) != mActorsKill.end();
			}),
		mActors.end());

	for (auto& it : mActorsKill)
	{
		it->End();
		TUDO_MEM_FREE(it);
	}
	mActorsKill.clear();
}

ActorFactory& World::GetActorFactory()
{
	return mFactory;
}

void World::BeginActors()
{
	bBegin = false;

	for (auto& it : mActorsBegin)
		it->Begin();
	mActorsBegin.clear();
}

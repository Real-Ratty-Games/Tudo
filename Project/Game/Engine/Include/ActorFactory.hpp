/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef ACTORFACTORY_HPP_
#define ACTORFACTORY_HPP_
#include <SystemTypes.hpp>
#include <unordered_map>
#include <functional>
#include "Actor.hpp"

using namespace Tudo;

class ActorFactory
{
public:
	using Creator = std::function<Actor* ()>;

	template<typename T>
	void Register(const strg& name)
	{
		if (!mCreators.contains(name))
		{
			mCreators[name] = []() {
				return new T();
				};
		}
	}

	Actor* Create(const strg& name);

	friend class World;

private:
	std::unordered_map<strg, Creator> mCreators;
};
#endif
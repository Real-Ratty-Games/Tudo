/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef ASSETMANAGER_HPP_
#define ASSETMANAGER_HPP_
#include "SystemTypes.hpp"
#include "Memory.hpp"
#include "Logger.hpp"
#include <unordered_map>

namespace Tudo
{
	template<typename T>
	class AssetManager
	{
	public:
		~AssetManager()
		{
			Free();
		}

		std::unordered_map<strg, T*>& Items()
		{
			return mItems;
		}

		// give up ownership to manager
		// asset must have Name() implemented!
		bool AddItem(T* item)
		{
			strg name = item->Name();
			if (mItems.find(name) == mItems.end())
			{
				mItems[name] = item;
				return true;
			}
			else return false;
		}

		void Free()
		{
			for (auto& it : mItems)
				TUDO_MEM_FREE(it.second);
			mItems.clear();
		}

		void Free(const strg& name)
		{
			auto it = mItems.find(name);
			if (it != mItems.end())
			{
				TUDO_MEM_FREE(it->second);
				mItems.erase(it);
			}
		}

		T& Get(const strg& name)
		{
			auto it = mItems.find(name);
			if (it == mItems.end())
				Logger::Log("AssetManager::Get", "Item '" + name + "' not found!", ELogType::LERROR);
			return *(it->second);
		}

	private:
		std::unordered_map<strg, T*> mItems;
	};
}
#endif
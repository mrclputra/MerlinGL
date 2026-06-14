#pragma once

#include "Merlin/Core/Core.h"
#include "Asset.h"

#include <unordered_map>
#include <memory>
#include <typeindex>

namespace Merlin {
	class MERLIN_API AssetManager {
	public:
		AssetManager() = default;
		~AssetManager() = default;
	
		AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;

		template<typename T, typename ...Args>
		std::shared_ptr<T> Load(const std::string& path, Args&& ...args);

		template<typename T>
		std::shared_ptr<T> Get(const std::string& path) const;

		template<typename T, typename Func>
		void ForEach(Func&& func);

		void Clear() { m_Assets.clear(); }

	private:
		std::unordered_map<std::type_index, std::unordered_map<std::string, std::shared_ptr<Asset>>> m_Assets;
	};

	// Template implementations
	template<typename T, typename ...Args>
	std::shared_ptr<T> AssetManager::Load(const std::string& path, Args&& ...args) {
		static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset");

		auto& typeCache = m_Assets[T::GetStaticTypeIndex()];
		auto it = typeCache.find(path);
		if (it != typeCache.end()) {
			return std::static_pointer_cast<T>(it->second);
		}

		auto asset = std::make_shared<T>(path, std::forward<Args>(args)...);
		typeCache[path] = asset;
		return asset;
	}

	template<typename T>
	std::shared_ptr<T> AssetManager::Get(const std::string& path) const {
		auto typeIt = m_Assets.find(T::GetStaticTypeIndex());
		if (typeIt == m_Assets.end()) return nullptr;

		auto assetIt = typeIt->second.find(path);
		if (assetIt == typeIt->second.end()) return nullptr;

		return std::static_pointer_cast<T>(assetIt->second);
	}

	template<typename T, typename Func>
	void AssetManager::ForEach(Func&& func) {
		auto it = m_Assets.find(T::GetStaticTypeIndex());
		if (it == m_Assets.end()) return;
		for (auto& [key, asset] : it->second) {
			func(std::static_pointer_cast<T>(asset));
		}
	}
}
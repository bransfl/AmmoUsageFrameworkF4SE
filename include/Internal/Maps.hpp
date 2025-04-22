#pragma once

#include <RE/Bethesda/MagicItems.hpp>
#include <unordered_map>

namespace Internal::Maps
{
	// form : ammo usage amount
	static std::unordered_map<RE::TESObjectWEAP*, uint32_t> weaponMap;
	static std::unordered_map<RE::BGSKeyword*, uint32_t> keywordMap;
} // namespace Internal::Maps

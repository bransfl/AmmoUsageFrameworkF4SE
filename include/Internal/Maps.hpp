#pragma once

#include <unordered_map>

namespace Internal::Maps
{
	// holds the parsed data before its loaded into actual maps
	static std::unordered_map<std::string, std::string> weaponPrepMap;
	static std::unordered_map<std::string, std::string> keywordPrepMap;

	// form : ammo usage amount
	static std::unordered_map<RE::TESObjectWEAP*, uint32_t> weaponMap;
	static std::unordered_map<RE::BGSKeyword*, uint32_t> keywordMap;
} // namespace Internal::Maps

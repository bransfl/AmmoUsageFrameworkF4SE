#pragma once
#include <RE/Bethesda/MagicItems.hpp>
#include <unordered_map>

namespace Internal::Maps
{
	// form : ammo usage amount
	std::unordered_map<RE::EnchantmentItem*, uint32_t> enchMap;
	std::unordered_map<RE::TESObjectWEAP*, uint32_t> weapMap;
} // namespace Internal::Maps

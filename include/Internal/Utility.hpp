#pragma once

namespace Internal
{
	class Utility
	{
	public:
		/**
		 * @brief brief
		 *
		 * @details details
		 * @param a_weapon a_weapons
		 * @return The amount of ammo to use.
		 *		   Returns 1 if the weapon or enchantments were not found in the map.
		 * 		   Returns >1 if the weapon or enchantments were found in the map.
		 */
		static uint32_t GetWeaponDataFromMaps(RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon);

		/**
		 * @brief Gets all keywords of the given weapon object instance.
		 *
		 * @param a_weapon a_weapon
		 * @return return
		 */
		static std::vector<RE::BGSKeyword*> GetWeaponKeywords(RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon);
	};
} // namespace Internal

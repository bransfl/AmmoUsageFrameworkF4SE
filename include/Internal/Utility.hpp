#pragma once

namespace Internal
{
	class Utility
	{
	public:
		/**
		 * @brief brief
		 * @details details
		 * @param a_weapon a_weapons
		 * @return Returns 1 if the weapon or enchantments were not found in the map.
		 * 		   Returns >1 if the weapon or enchantments were found in the map.
		 */
		static uint32_t IsWeaponDataInMap(RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon);
	};
} // namespace Internal

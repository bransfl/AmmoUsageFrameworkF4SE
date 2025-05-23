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
		static uint32_t GetWeaponDataFromMaps(const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon);

		/**
		 * @brief Gets all keywords of the given weapon object instance.
		 *
		 * @param a_weapon a_weapon
		 * @return return
		 */
		static std::vector<RE::BGSKeyword*> GetWeaponKeywords(const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon);

		/**
		 * @brief Returns the amount of ammo loaded in the given actor's equipped gun's magazine.
		 *
		 * @param a_this The actor to get the loaded ammo count from.
		 * @return The actor's magazine ammo count.
		 */
		static uint32_t GetWeaponLoadedAmmoCount(RE::Actor* a_this);

		/**
		 * @brief Returns the TESForm of the ammo loaded in the given actor's equipped gun's magazine.
		 *
		 * @param a_this The actor to get the loaded ammo form from.
		 * @return The actor's loaded ammo form.
		 */
		static RE::TESAmmo* GetWeaponLoadedAmmoForm(RE::Actor* a_this);

		/**
		 * @brief Parses INI file Keys into TESForms.
		 *
		 * @param a_identifier a_identifier.
		 * @return Return.
		 */
		static RE::TESForm* GetFormFromIdentifier(const std::string& identifier);

		/**
		 * @brief Brief.
		 *
		 * @param a_modName	The modname to check.
		 * @return True if the mod is installed.
		 * 		   False if he mod is not installed.
		 */
		static bool IsPluginInstalled(std::string_view a_modName);
	};
} // namespace Internal

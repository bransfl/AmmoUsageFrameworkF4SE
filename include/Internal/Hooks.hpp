#pragma once

namespace Internal
{
	class Hooks
	{
	public:
		/**
		 * @brief Installs the hook.
		 */
		static void Install() noexcept;

	private:
		/**
		 * @brief Hook that adjusts ammo usage for the amount of ammo in the parsed map.
		 *
		 * @param a_this The actor that is using ammo.
		 * @param a_weapon The instancedata of a_this's equipped weapon.
		 * @param a_equipIndex The equipindex of a_weapon.
		 * @param a_shotCount The amount of ammo used per shot.
		 * @return The final amount of ammo the actor will have after firing..
		 */
		static uint32_t Hook_UseAmmo(RE::Actor* a_this, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, std::uint32_t a_shotCount);
	};
}

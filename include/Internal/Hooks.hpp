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
		 * @brief brief
		 * @details details
		 * @param a_this The actor that is using ammo.
		 * @param a_weapon The instancedata of a_this's equipped weapon.
		 * @param a_equipIndex The equipindex of a_weapon.
		 * @param a_shotCount The amount of ammo used per shot.
		 * @return return
		 */
		static uint32_t Hook_UseAmmo(RE::Actor* a_this, RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, uint32_t a_shotCount);
	};
} // namespace Internal

// virtual std::uint32_t UseAmmo(const BGSObjectInstanceT<TESObjectWEAP>& a_weapon, BGSEquipIndex a_equipIndex, std::uint32_t a_shotCount);														// 0F0

#pragma once

#define WEAPON_TYPE_GUN 9

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
		 * @details Details.
		 * @param a_this The actor that is using ammo.
		 * @param a_weapon The instancedata of a_this's equipped weapon.
		 * @param a_equipIndex The equipindex of a_weapon.
		 * @param a_shotCount The amount of ammo used per shot.
		 * @return Return.
		 */
		static uint32_t Hooked_Actor_UseAmmo(RE::Actor* a_this, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, std::uint32_t a_shotCount);
	};
} // namespace Internal

// virtual std::uint32_t UseAmmo(const BGSObjectInstanceT<TESObjectWEAP>& a_weapon, BGSEquipIndex a_equipIndex, std::uint32_t a_shotCount);														// 0F0
// inline constexpr std::array<REL::ID, 17> Actor{ REL::ID(1455516), REL::ID(985925), REL::ID(61957), REL::ID(720550), REL::ID(1378017), REL::ID(453840), REL::ID(48089), REL::ID(1145017), REL::ID(613231), REL::ID(519904), REL::ID(1383399), REL::ID(459179), REL::ID(1117514), REL::ID(193789), REL::ID(851552), REL::ID(1510044), REL::ID(585842) };

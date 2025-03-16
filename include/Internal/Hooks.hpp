#pragma once

namespace Internal
{
	class Hooks
	{
	public:
		/**
		 * @brief brief
		 */
		static void Install() noexcept;

	private:
		/**
		 * @brief brief
		 * @details details
		 * @param a_this a_this
		 * @param a_weapon a_weapon
		 * @param a_equipIndex a_equipIndex
		 * @param a_shotCount a_shotCount
		 * @return return
		 */
		static uint32_t Hook_UseAmmo(RE::Actor* a_this, RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, uint32_t a_shotCount);
	};
} // namespace Internal

// virtual std::uint32_t UseAmmo(const BGSObjectInstanceT<TESObjectWEAP>& a_weapon, BGSEquipIndex a_equipIndex, std::uint32_t a_shotCount);														// 0F0

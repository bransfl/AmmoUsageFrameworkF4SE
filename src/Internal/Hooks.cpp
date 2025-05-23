#include "Internal/Hooks.hpp"
#include "Internal/Utility.hpp"
#include <detourXS/detourxs.h>

#pragma warning(disable : 4100)

namespace Internal
{
	typedef uint32_t(UseAmmo_Signature)(RE::Actor*, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>&, RE::BGSEquipIndex, std::uint32_t);
	REL::Relocation<UseAmmo_Signature> UseAmmo_OriginalFunction;
	DetourXS DetourxsHook;

	void Hooks::Install() noexcept
	{
		logger::info("Hook installing..."sv);

		if (REL::Module::IsNG()) {
			// next-gen hook
			REL::Relocation<UseAmmo_Signature> Actor_UseAmmo_Location{ REL::ID(2231061) };
			if (DetourxsHook.Create(reinterpret_cast<LPVOID>(Actor_UseAmmo_Location.address()), &Hook_UseAmmo)) {
				UseAmmo_OriginalFunction = reinterpret_cast<std::uintptr_t>(DetourxsHook.GetTrampoline());
				logger::info("Successfully installed Actor::UseAmmo hook for Next-Gen"sv);
			}
			else {
				logger::info("Failed to install Actor::UseAmmo hook for Next-Gen"sv);
			}
		}
		else {
			// last-gen hook
			REL::Relocation<UseAmmo_Signature> Actor_UseAmmo_Location{ REL::ID(228455) };
			if (DetourxsHook.Create(reinterpret_cast<LPVOID>(Actor_UseAmmo_Location.address()), &Hook_UseAmmo)) {
				UseAmmo_OriginalFunction = reinterpret_cast<std::uintptr_t>(DetourxsHook.GetTrampoline());
				logger::info("Successfully installed Actor::UseAmmo hook for Last-Gen"sv);
			}
			else {
				logger::info("Failed to install Actor::UseAmmo hook for Last-Gen"sv);
			}
		}

		logger::info("Hook installed."sv);
	}

	uint32_t Hooks::Hook_UseAmmo(RE::Actor* a_this, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, std::uint32_t a_shotCount)
	{
		// IMPORTANT: we have to return the final amount of ammo that will be in the magazine after firing

		logger::info("Hook_UseAmmo() ran. Initial a_shotCount: {}"sv, a_shotCount);

		if (!a_this) {
			logger::warn("Hooks::Hook_UseAmmo::a_this was nullptr, return default value"sv);
			return (a_shotCount > 0) ? a_shotCount-- : 0;
		}

		auto* player = RE::PlayerCharacter::GetSingleton();
		if (a_this != player) {
			logger::info("Hooks::Hook_UseAmmo::a_this was not the player, return default value"sv);
			return (a_shotCount > 0) ? a_shotCount-- : 0;
		}

		uint32_t result = 0;
		// check weapon data for ammo usage amount
		if (a_weapon.object != nullptr && a_weapon.object->formType == RE::ENUM_FORMTYPE::kWEAP) {
			// setup weapon data
			RE::TESObjectWEAP* weapon = (RE::TESObjectWEAP*)a_weapon.object;
			RE::TESObjectWEAP::InstanceData* weaponData = (RE::TESObjectWEAP::InstanceData*)a_weapon.instanceData.get();
			uint8_t weaponType = weapon->weaponData.type.underlying();
			if (weaponData) {
				weaponType = weaponData->type.underlying();
			}
			uint32_t loadedAmmoCount = Utility::GetWeaponLoadedAmmoCount(a_this);
			bool weaponChargingReload = weaponData->flags.any(RE::WEAPON_FLAGS::kChargingReload);

			// temp info logging
			logger::info("Hooks::Hook_UseAmmo() -> Player Weapon Info: (FormID: {:08X}), (EditorID: {}), (weaponType: {}), (ammoCapacity: {}), (loadedAmmoCount: {}), (weaponChargingReload: {})"sv,
				weapon->GetFormID(), weapon->GetFormEditorID(), weaponType, weaponData->ammoCapacity, loadedAmmoCount, weaponChargingReload);

			if (weaponType != 9) {
				logger::warn("Hooks::Hook_UseAmmo() -> Player Weapon was not a gun, return default value"sv);
				return (loadedAmmoCount > 0) ? loadedAmmoCount-- : 0;
			}

			// determine the amount of ammo to use
			uint32_t mapsShotCount = Utility::GetWeaponDataFromMaps(a_weapon);
			if (mapsShotCount > 1) {
				// weapon data was found in Maps
				result = (loadedAmmoCount >= mapsShotCount) ? loadedAmmoCount - mapsShotCount : 0;
			}
			else {
				// weapon data was not found in Maps
				if (weaponChargingReload == true) {
					logger::info("Hooks::Hook_UseAmmo() -> Player Weapon has charging reload flag"sv);
					result = 0;
				}
				else {
					// normal weapon
					logger::info("Hooks::Hook_UseAmmo() -> Player Weapon does not have charging reload flag"sv);
					result = (loadedAmmoCount > 0) ? loadedAmmoCount - 1 : 0;
				}
			}
			logger::info("Hooks::Hook_UseAmmo() -> Utility::GetWeaponDataFromMaps() was processed, result is now: {}"sv, result);

			// remove ammo from the player - this is so cringe and ugly
			uint32_t totalAmmoCount = 0;
			player->GetItemCount(totalAmmoCount, weaponData->ammo, false);
			uint32_t totalAmmoToRemove = 1;

			if (mapsShotCount > 1 && totalAmmoCount >= mapsShotCount) {
				// we have enough ammo
				totalAmmoToRemove = mapsShotCount;
			}
			else {
				// we dont have enough ammo
				if (weaponChargingReload) {
					// remove the player's remaining ammo
					totalAmmoToRemove = loadedAmmoCount;
				}
				else {
					// remove 1 ammo as normal
					totalAmmoToRemove = mapsShotCount;
				}
			}

			player->RemoveItem(*new RE::TESObjectREFR::RemoveItemData(weaponData->ammo, totalAmmoToRemove));
		}

		logger::info("debug: end of hook, returning result: {}"sv, result);
		return result;
	}
}

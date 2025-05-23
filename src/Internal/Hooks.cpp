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

		// uint32_t ret; // TODO make this the final value to return at the end
		//
		uint32_t totalAmmoCount = 1;
		uint32_t newShotCount = 1;
		// check weapon data for ammo usage amount
		if (a_weapon.object != nullptr && a_weapon.object->formType == RE::ENUM_FORMTYPE::kWEAP) {
			// weapon data
			RE::TESObjectWEAP* weapon = (RE::TESObjectWEAP*)a_weapon.object;
			RE::TESObjectWEAP::InstanceData* weaponData = (RE::TESObjectWEAP::InstanceData*)a_weapon.instanceData.get();
			uint8_t weaponType = weapon->weaponData.type.underlying();
			if (weaponData) {
				weaponType = weaponData->type.underlying();
			}
			uint32_t loadedAmmoCount = Utility::GetWeaponLoadedAmmoCount(a_this);

			// temp weapon info logging
			logger::info("player's a_weapon -> formid: {:08X}, editorid: {}, weaponType: {}, ammoCapacity: {}, loadedAmmoCount: {}"sv,
				weapon->GetFormID(), weapon->GetFormEditorID(), weaponType, weaponData->ammoCapacity, loadedAmmoCount);

			// we only care about guns, no melee weapons allowed
			if (weaponType != 9) {
				logger::warn("Hooks::Hook_UseAmmo::weaponType was not a gun, return default value"sv);
				return (a_shotCount > 0) ? a_shotCount-- : 0;
			}

			// check amount of ammo to use from maps
			uint32_t mapShotCount = Utility::GetWeaponDataFromMaps(a_weapon);
			if (mapShotCount > 1) {
				// was found in maps
				// newShotCount = loadedAmmoCount - mapShotCount;
				if (mapShotCount < loadedAmmoCount) {
					newShotCount = loadedAmmoCount - mapShotCount;
				}
				else {
					newShotCount = mapShotCount;
				}
			}
			else {
				// not found in maps
				newShotCount = loadedAmmoCount - 1;
			}
			logger::info("Utility::GetWeaponDataFromMaps on weapon {} produced {}"sv,
				weapon->GetFormEditorID(), newShotCount);

			// ammo removal
			// todo - clean this entire section up + fix it so it doesnt underflow as a uint32_t
			logger::info("About to remove ammo..."sv);
			totalAmmoCount;
			uint32_t totalAmmoToRemove;
			player->GetItemCount(totalAmmoCount, weaponData->ammo, false);
			logger::info("player total ammo count before firing: {}"sv, totalAmmoCount);
			if (totalAmmoCount < mapShotCount) {
				totalAmmoToRemove = totalAmmoCount;
			}
			else {
				totalAmmoToRemove = mapShotCount;
			}
			logger::info("totalAmmoToRemove: {}"sv, totalAmmoToRemove);
			auto* removeItemData = new RE::TESObjectREFR::RemoveItemData(weaponData->ammo, totalAmmoToRemove);
			player->RemoveItem(*removeItemData);
			player->GetItemCount(totalAmmoCount, weaponData->ammo, false);
			logger::info("player total ammo count after firing: {}"sv, totalAmmoCount);
		}

		// return UseAmmo_OriginalFunction(a_this, a_weapon, a_equipIndex, a_shotCount);
		logger::info("debug: end of hook, returning totalAmmoCount: {}"sv, totalAmmoCount);
		return totalAmmoCount;
	}
}

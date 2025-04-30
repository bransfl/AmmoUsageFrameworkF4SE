#include "Internal/Hooks.hpp"
#include "Internal/Utility.hpp"
#include <detourXS/detourxs.h>

namespace Internal
{
	typedef uint32_t(Signature_UseAmmo)(RE::Actor*, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>&, RE::BGSEquipIndex, uint32_t);
	REL::Relocation<Signature_UseAmmo> OriginalFunction_UseAmmo;
	DetourXS UseAmmoHook;

	void Hooks::Install() noexcept
	{
		logger::info("Hook installing..."sv);

		// F4SE::Trampoline& trampoline = F4SE::GetTrampoline();

		// if (REL::Module::IsNG()) {
		// 	// Next-Gen
		// 	REL::Relocation<uintptr_t> ptr_UseAmmo_NG{ REL::ID(2231061) };
		// 	OriginalFunction_UseAmmo = trampoline.write_call<5>(ptr_UseAmmo_NG.address(), &Hook_UseAmmo);
		// }
		// else {
		// 	// Last-Gen
		// 	REL::Relocation<uintptr_t> ptr_UseAmmo_OG{ REL::ID(228455) };
		// 	OriginalFunction_UseAmmo = trampoline.write_call<5>(ptr_UseAmmo_OG.address(), &Hook_UseAmmo);
		// }

		if (REL::Module::IsNG()) {
			REL::Relocation<Signature_UseAmmo> UseAmmoLocationNG{ REL::ID(2231061) };
			if (UseAmmoHook.Create(reinterpret_cast<LPVOID>(UseAmmoLocationNG.address()), &Hook_UseAmmo)) {
				OriginalFunction_UseAmmo = reinterpret_cast<std::uintptr_t>(UseAmmoHook.GetTrampoline());
			}
			else {
				logger::warn("Failed to create Next-Gen Hook"sv);
			}
		}
		else {
			REL::Relocation<Signature_UseAmmo> UseAmmoLocationOG{ REL::ID(228455) };
			if (UseAmmoHook.Create(reinterpret_cast<LPVOID>(UseAmmoLocationOG.address()), &Hook_UseAmmo)) {
				OriginalFunction_UseAmmo = reinterpret_cast<std::uintptr_t>(UseAmmoHook.GetTrampoline());
			}
			else {
				logger::warn("Failed to create Last-Gen Hook"sv);
			}
		}

		logger::info("Hook installed."sv);
	}

	uint32_t Hooks::Hook_UseAmmo(RE::Actor* a_this, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, uint32_t a_shotCount)
	{
		logger::info("hook ran"sv);

		if (a_this != RE::PlayerCharacter::GetSingleton()) {
			// we only need this to run for the player
			return OriginalFunction_UseAmmo(a_this, a_weapon, a_equipIndex, a_shotCount);
		}

		if (a_weapon.object != nullptr && a_weapon.object->formType == RE::ENUM_FORMTYPE::kWEAP) {
			RE::TESObjectWEAP* weapon = (RE::TESObjectWEAP*)a_weapon.object;
			RE::TESObjectWEAP::Data* data = (RE::TESObjectWEAP::Data*)a_weapon.instanceData.get();

			logger::info("player's a_weapon form: {:08X}, {}"sv,
				weapon->GetFormID(), weapon->GetFormEditorID());

			uint8_t weaponType = weapon->weaponData.type.underlying();
			if (data) {
				weaponType = data->type.underlying();
			}

			logger::info("player's a_weapon - formid: {:08X}, editorid: {}, weaponType: {}"sv,
				weapon->GetFormID(), weapon->GetFormEditorID(), weaponType);

			// we only care about guns. no melee weapons allowed
			if (weaponType != WEAPON_TYPE_GUN) {
				return OriginalFunction_UseAmmo(a_this, a_weapon, a_equipIndex, a_shotCount);
			}

			// ammo count is set here
			a_shotCount = Utility::GetWeaponDataFromMaps(a_weapon);
			logger::info("Utility::GetWeaponDataFromMaps on weapon {} returned {}"sv,
				weapon->GetFormEditorID(), a_shotCount);
		}

		return OriginalFunction_UseAmmo(a_this, a_weapon, a_equipIndex, a_shotCount);
	}
} // namespace Internal

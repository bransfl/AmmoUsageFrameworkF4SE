#include "Internal/Hooks.hpp"
#include "Internal/Utility.hpp"
#include <detourXS/detourxs.h>

namespace Internal
{
	// typedef uint32_t(Signature_UseAmmo)(RE::Actor*, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>&, RE::BGSEquipIndex, uint32_t);
	// REL::Relocation<Signature_UseAmmo> OriginalFunction_UseAmmo;
	// DetourXS UseAmmoHook;
	// using UseAmmo_Signature = decltype(&RE::Actor::UseAmmo);
	typedef uint32_t(UseAmmo_Signature)(RE::Actor*, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>&, RE::BGSEquipIndex, std::uint32_t);
	REL::Relocation<UseAmmo_Signature> UseAmmo_OriginalFunction;
	DetourXS DetourxsHook;

	void Hooks::Install() noexcept
	{
		logger::info("Hook installing..."sv);

		// REL::Relocation<UseAmmo_Signature> Actor_UseAmmo_Location{ REL::ID(228455) };
		// if (DetourxsHook.Create(reinterpret_cast<LPVOID>(Actor_UseAmmo_Location.address()), &Hook_UseAmmo)) {
		// 	UseAmmo_OriginalFunction = reinterpret_cast<std::uintptr_t>(DetourxsHook.GetTrampoline());
		// 	logger::info("Installed Actor::UseAmmo hook"sv);
		// }
		// else {
		// 	logger::warn("Failed to install Actor::UseAmmo hook"sv);
		// }

		logger::info("Hook was manually disabled for now"sv);
	}

	uint32_t Hooks::Hook_UseAmmo(RE::Actor* a_this, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, std::uint32_t a_shotCount)
	{
		logger::info("hook ran"sv);

		// if (a_this != RE::PlayerCharacter::GetSingleton()) {
		// 	// we only need this to run for the player
		// 	return Actor_UseAmmo_OriginalFunction(a_this, a_weapon, a_equipIndex, a_shotCount);
		// }

		// if (a_weapon.object != nullptr && a_weapon.object->formType == RE::ENUM_FORMTYPE::kWEAP) {
		// 	RE::TESObjectWEAP* weapon = (RE::TESObjectWEAP*)a_weapon.object;
		// 	RE::TESObjectWEAP::InstanceData* data = (RE::TESObjectWEAP::InstanceData*)a_weapon.instanceData.get();

		// 	logger::info("player's a_weapon form: {:08X}, {}"sv,
		// 		weapon->GetFormID(), weapon->GetFormEditorID());

		// 	uint8_t weaponType = weapon->weaponData.type.underlying();
		// 	if (data) {
		// 		weaponType = data->type.underlying();
		// 	}

		// 	logger::info("player's a_weapon - formid: {:08X}, editorid: {}, weaponType: {}"sv,
		// 		weapon->GetFormID(), weapon->GetFormEditorID(), weaponType);

		// 	// we only care about guns. no melee weapons allowed
		// 	if (weaponType != WEAPON_TYPE_GUN) {
		// 		return Actor_UseAmmo_OriginalFunction(a_this, a_weapon, a_equipIndex, a_shotCount);
		// 	}

		// 	// ammo count is set here (ugly bc i was testing logging)
		// 	uint32_t newShotCount = Utility::GetWeaponDataFromMaps(a_weapon);
		// 	if (newShotCount > 1) {
		// 		a_shotCount = newShotCount;
		// 	}
		// 	else {
		// 		//
		// 	}
		// 	logger::info("Utility::GetWeaponDataFromMaps on weapon {} returned {}"sv,
		// 		weapon->GetFormEditorID(), newShotCount);
		// }

		logger::info("Hook is about to return original function..."sv);
		return UseAmmo_OriginalFunction(a_this, a_weapon, a_equipIndex, a_shotCount);
	}
} // namespace Internal

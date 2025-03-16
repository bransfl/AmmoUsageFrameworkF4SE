#include "Internal/Hooks.hpp"
#include "f4se/GameApi.h"
#include <f4se_common/BranchTrampoline.h>
#include <f4se_common/Relocation.h>

namespace Internal
{
	typedef uint32_t(Signature_UseAmmo)(RE::Actor*, RE::BGSObjectInstanceT<RE::TESObjectWEAP>&, RE::BGSEquipIndex, uint32_t);
	REL::Relocation<Signature_UseAmmo> OriginalFunction_UseAmmo;

	// RelocAddr<Signature_UseAmmo> UseAmmo_FunctionOG(0x0021E420);

	// RelocPtr<void> UseAmmo_DestOG(0x00DFD890);
	// RelocPtr<void> UseAmmo_DestNG(0x00C486A0);

	// REL::Relocation<mem_AddScriptAddedLeveledObjectSig> OriginalFunction_AddScriptAddedLeveledObject;
	void Hooks::Install() noexcept
	{
		logger::info("Hook installing..."sv);

		// F4SE::Trampoline& trampoline = F4SE::GetTrampoline();

		if (REL::Module::IsNG()) {
			// NG Patch - TODO needs to be tested
			logger::info("Hook aborted. Reason: Game version was Next-Gen."sv);
			return;
		}
		else {
			// OG Patch
			// OriginalFunction_UseAmmo = (Signature_UseAmmo)code.getCode();

			// g_branchTrampoline.Write5Branch((uintptr_t)InterReloc::UseAmmo_Modded, (uintptr_t)UseAmmo_Hook);
			// REL::Relocation<uintptr_t> ptr_UseAmmo_OG{ REL::ID(553651) };
			// OriginalFunction_UseAmmo = trampoline.write_branch<5>(ptr_UseAmmo_OG.address(), &Hook_UseAmmo);

			// if (!g_branchTrampoline.Create(14)) {
			// 	return;
			// }
			// g_branchTrampoline.Write5Call(UseAmmo_DestOG.GetUIntPtr(), (uintptr_t)Hook_UseAmmo);
		}

		logger::info("Hook installed."sv);
	}

	// potential address: 0x872B3 or 0xC4C7C64 or 0x4D07820
	// sylee's UseAmmo addresses - OG: 0x00DFD890, NG: 0x00C486A0 - maybe add 0x05 to these addresses?
	//
	uint32_t Hooks::Hook_UseAmmo(RE::Actor* a_this, RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, uint32_t a_shotCount)
	{
		logger::info("hook ran"sv);

		if (a_weapon.object != nullptr && a_weapon.object->formType == RE::ENUM_FORMTYPE::kWEAP) {
			RE::TESObjectWEAP* weap = (RE::TESObjectWEAP*)a_weapon.object;
			RE::TESObjectWEAP::Data* data = (RE::TESObjectWEAP::Data*)a_weapon.instanceData.get();

			uint8_t type = weap->weaponData.type.underlying();

			if (data != nullptr) {
				type = data->type.underlying();
			}

			// check the map from the parser for the weap's form or omods in the instance data, and set if needed

			logger::info("weapon: {:08X}, {}."sv, weap->GetFormID(), weap->GetFormEditorID());

			// we only care about guns, not melee weapons
			if (type == 9) {
				// a_shotCount = static_cast<uint32_t>(reach);
				logger::info("a_count set to {}."sv, a_shotCount);
			}
		}

		return OriginalFunction_UseAmmo(a_this, a_weapon, a_equipIndex, a_shotCount);
	}
} // namespace Internal

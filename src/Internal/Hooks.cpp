#include "Internal/Hooks.hpp"
#include "f4se/GameApi.h"
#include <f4se_common/BranchTrampoline.h>
#include <f4se_common/Relocation.h>

namespace Internal
{
	typedef uint32_t (*Signature_UseAmmo)(RE::Actor*, RE::BGSObjectInstanceT<RE::TESObjectWEAP>&, RE::BGSEquipIndex, uint32_t);
	REL::Relocation<Signature_UseAmmo> OriginalFunction_UseAmmo;

	// OG Patch
	// int tmp = 0x13B79C0;
	// REL::Relocation<uintptr_t> locationUseAmmo_OG{ REL::ID(26930228) };
	// REL::Relocation<uintptr_t> ptr_UseAmmo_OG = REL::Relocation(0x1413B79C0);
	// OriginalFunction_UseAmmo = trampoline.write_branch<5>(locationUseAmmo_OG.address(), &Hook_UseAmmo);
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
			// uintptr_t addr = RE::VTABLE::Actor[16].address() + 8 * 0x05;
			// REL::safe_write(addr, (uintptr_t)Hook_UseAmmo);
		}

		logger::info("Hook installed."sv);
	}

	// potential address: 0x13B79C0 or 0x872B3 or 0xC4C7C64 or 0x4D07820 or 0x19AEC40
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
			// this needs to be polished
			auto* player = RE::PlayerCharacter::GetSingleton();
			RE::EquippedItem& equipped = player->currentProcess->middleHigh->equippedItems[0];
			RE::TESObjectWEAP* weapForm = (RE::TESObjectWEAP*)equipped.item.object;
			RE::TESObjectWEAP::InstanceData* instance = (RE::TESObjectWEAP::InstanceData*)equipped.item.instanceData.get();
			RE::EquippedWeaponData* weapData = (RE::EquippedWeaponData*)equipped.data.get();
			

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

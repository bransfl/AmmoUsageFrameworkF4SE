#include "Internal/Hooks.hpp"
#include "Internal/Utility.hpp"
#include "f4se/GameApi.h"
#include <f4se_common/BranchTrampoline.h>
#include <f4se_common/Relocation.h>

namespace Internal
{
	typedef uint32_t (*Signature_UseAmmo)(RE::Actor*, RE::BGSObjectInstanceT<RE::TESObjectWEAP>&, RE::BGSEquipIndex, uint32_t);
	REL::Relocation<Signature_UseAmmo> OriginalFunction_UseAmmo;

	static constexpr uint8_t WEAPON_TYPE_GUN = 9; // laziness

	template <class F, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[0] };
		T::func = vtbl.write_vfunc(T::size, T::thunk);
	}

	struct UseAmmoStruct
	{
		static uint32_t thunk(RE::Actor*, RE::BGSObjectInstanceT<RE::TESObjectWEAP>&, RE::BGSEquipIndex, uint32_t)
		{
			logger::info("ammo used"sv);
			return 1;
		}
		[[maybe_unused]] static inline REL::Relocation<decltype(thunk)> func;

		static inline size_t size = 0x2B;
	};
	// write_vfunc<RE::Actor, UseAmmoStruct>();
	//  553651
	void Hooks::Install() noexcept
	{
		logger::info("Hook installing..."sv);

		F4SE::Trampoline& trampoline = F4SE::GetTrampoline();

		if (REL::Module::IsNG()) {
			// NG Patch - TODO needs to be tested
			logger::info("Hook aborted. Reason: Game version was Next-Gen."sv);
			return;
		}
		else {
			// uintptr_t addr = RE::VTABLE::Actor[16].address() + 8 * 0x05;
			// REL::safe_write(addr, (uintptr_t)Hook_UseAmmo);
			REL::Relocation<uintptr_t> target_OG{ REL::ID(553651) };
			OriginalFunction_UseAmmo = trampoline.write_branch<5>(target_OG.address(), &Hook_UseAmmo);
		}

		logger::info("Hook installed."sv);
	}

	// struct CheckValidTarget
	// {
	// 	static bool thunk(RE::Actor*, RE::TESObjectREFR&)
	// 	{
	// 		return true;
	// 	}
	// 	[[maybe_unused]] static inline REL::Relocation<decltype(thunk)> func;

	// 	static inline size_t size = 0x0D6;
	// };

	// void Install()
	// {
	// 	stl::write_vfunc<RE::Character, CheckValidTarget>();
	// }

	// OG Patch
	// int tmp = 0x13B79C0;
	// REL::Relocation<uintptr_t> locationUseAmmo_OG{ REL::ID(26930228) };
	// REL::Relocation<uintptr_t> ptr_UseAmmo_OG = REL::Relocation(0x1413B79C0);
	// OriginalFunction_UseAmmo = trampoline.write_branch<5>(locationUseAmmo_OG.address(), &Hook_UseAmmo);

	// potential address: 0x13B79C0 or 0x872B3 or 0xC4C7C64 or 0x4D07820 or 0x19AEC40
	// sylee's UseAmmo addresses - OG: 0x00DFD890, NG: 0x00C486A0 - maybe add 0x05 to these addresses?
	//

	uint32_t Hooks::Hook_UseAmmo(RE::Actor* a_this, RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, uint32_t a_shotCount)
	{
		logger::info("hook ran"sv);

		if (a_weapon.object != nullptr && a_weapon.object->formType == RE::ENUM_FORMTYPE::kWEAP) {
			RE::TESObjectWEAP* weap = (RE::TESObjectWEAP*)a_weapon.object;
			RE::TESObjectWEAP::Data* data = (RE::TESObjectWEAP::Data*)a_weapon.instanceData.get();

			logger::info("weapon: {:08X}, {}."sv, weap->GetFormID(), weap->GetFormEditorID());

			uint8_t type = weap->weaponData.type.underlying();
			if (data != nullptr) {
				type = data->type.underlying();
			}

			// we only care about guns, not melee weapons
			if (type != WEAPON_TYPE_GUN) {
				return OriginalFunction_UseAmmo(a_this, a_weapon, a_equipIndex, a_shotCount);
			}

			// ammo count is set here
			uint32_t result = Utility::IsWeaponDataInMap(a_weapon);
			if (result > 1) {
				a_shotCount = result;
				logger::info("a_count set to {}."sv, a_shotCount);
			}
		}
		return OriginalFunction_UseAmmo(a_this, a_weapon, a_equipIndex, a_shotCount);
	}
} // namespace Internal

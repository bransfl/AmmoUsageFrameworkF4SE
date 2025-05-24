#include "Internal/Hooks.hpp"
#include "Internal/Utility.hpp"
// #include <detourXS/detourxs.h>

#pragma warning(disable : 4100)

namespace Internal
{
	typedef uint32_t(UseAmmo_Signature)(RE::Actor*, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>&, RE::BGSEquipIndex, std::uint32_t);
	REL::Relocation<UseAmmo_Signature> UseAmmo_OriginalFunction;
	// DetourXS DetourXS_Hook;

	void Hooks::Install() noexcept
	{
		// OG: 228455
		// NG: 2231061
		logger::info("Hook installing..."sv);

		// virtual std::uint32_t UseAmmo(const BGSObjectInstanceT<TESObjectWEAP>& a_weapon, BGSEquipIndex a_equipIndex, std::uint32_t a_shotCount);	// 0F0
		// uintptr_t addr = RE::Actor::VTABLE[0].address() + (8 * 0x0F0);
		// REL::safe_write(addr, (uintptr_t)Hooks::UseAmmo);

		auto& trampoline = F4SE::GetTrampoline();
		trampoline.create(20);
		REL::Relocation<UseAmmo_Signature> UseAmmo_LocationOG{ REL::ID(228455) };
		UseAmmo_OriginalFunction = trampoline.write_branch<5>(UseAmmo_LocationOG.address(), &UseAmmo_Hook);

		// uintptr_t addr = RE::VTABLE::Actor[0].address() + (8 * 0x0F0);
		// REL::safe_write(addr, (uintptr_t)UseAmmo_HookALT);

		logger::info("Hook installed."sv);
	}

	uint32_t UseAmmo_HookALT(RE::Actor* a_this, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, std::uint32_t a_shotCount)
	{
		logger::info("UseAmmo_HookALT() ran"sv);
		return 1;
	}

	uint32_t Hooks::UseAmmo_Hook(RE::Actor* a_this, const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, std::uint32_t a_shotCount)
	{
		// IMPORTANT: we have to return the final amount of ammo that will be in the magazine after firing

		logger::info("Hooks::UseAmmo_Hook() ran. Initial a_shotCount: {}"sv, a_shotCount);

		return UseAmmo_OriginalFunction(a_this, a_weapon, a_equipIndex, a_shotCount);
	}
}

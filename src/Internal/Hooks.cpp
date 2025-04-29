#include "Internal/Hooks.hpp"
#include "Internal/Utility.hpp"



namespace Internal
{
	typedef uint32_t (*Signature_UseAmmo)(RE::Actor*, RE::BGSObjectInstanceT<RE::TESObjectWEAP>&, RE::BGSEquipIndex, uint32_t);
	REL::Relocation<Signature_UseAmmo> OriginalFunction_UseAmmo;

	void Hooks::Install() noexcept
	{
		logger::info("Hook installing..."sv);

		if (REL::Module::IsNG()) {
			// Next-Gen
			//
		}
		else {
			// Last-Gen
			//
		}

		logger::info("Hook installed."sv);
	}

	uint32_t Hooks::Hook_UseAmmo(RE::Actor* a_this, RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon, RE::BGSEquipIndex a_equipIndex, uint32_t a_shotCount)
	{
		logger::info("hook ran"sv);

		if (a_weapon.object != nullptr && a_weapon.object->formType == RE::ENUM_FORMTYPE::kWEAP) {
			RE::TESObjectWEAP* weapon = (RE::TESObjectWEAP*)a_weapon.object;
			RE::TESObjectWEAP::Data* data = (RE::TESObjectWEAP::Data*)a_weapon.instanceData.get();

			logger::info("player's a_weapon form: {:08X}, {}."sv,
				weapon->GetFormID(), weapon->GetFormEditorID());

			uint8_t weaponType = weapon->weaponData.type.underlying();
			if (data) {
				weaponType = data->type.underlying();
			}

			logger::info("player's a_weapon - formid: {:08X}, editorid: {}, weaponType: {}."sv,
				weapon->GetFormID(), weapon->GetFormEditorID(), weaponType);

			// we only care about guns. no melee weapons allowed
			if (weaponType != WEAPON_TYPE_GUN) {
				return OriginalFunction_UseAmmo(a_this, a_weapon, a_equipIndex, a_shotCount);
			}

			// ammo count is set here
			a_shotCount = Utility::GetWeaponDataFromMaps(a_weapon);
			logger::info("argument a_count set to {}."sv, a_shotCount);
		}

		return OriginalFunction_UseAmmo(a_this, a_weapon, a_equipIndex, a_shotCount);
	}
} // namespace Internal

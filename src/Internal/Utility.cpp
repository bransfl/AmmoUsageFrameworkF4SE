#include "Internal/Utility.hpp"
#include "Internal/Maps.hpp"
#include <string>

namespace Internal
{
	uint32_t Utility::GetWeaponDataFromMaps(const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon)
	{
		if (Maps::keywordDataMap.empty() && Maps::weaponDataMap.empty()) {
			logger::info("Utility::GetWeaponDataFromMaps - both DataMaps were empty, return 1"sv);
			return 1;
		}

		// check keywords first, since they have priority
		std::vector<RE::BGSKeyword*> weaponKeywords = Utility::GetWeaponKeywords(a_weapon);
		if (weaponKeywords.size() > 0) {
			for (RE::BGSKeyword* keyword : weaponKeywords) {
				if (Maps::keywordDataMap.contains(keyword)) {
					return Maps::keywordDataMap.at(keyword);
				}
			}
		}

		// maybe check objectmods?

		// check weapon form second, since this is a fallback
		RE::TESObjectWEAP* weaponForm = (RE::TESObjectWEAP*)a_weapon.object;
		if (weaponForm) {
			if (Maps::weaponDataMap.contains(weaponForm)) {
				return Maps::weaponDataMap.at(weaponForm);
			}
		}

		// the weapon/data wasn't found, return the original amount
		return 1;
	}

	std::vector<RE::BGSKeyword*> Utility::GetWeaponKeywords(const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon)
	{
		auto keywords = std::vector<RE::BGSKeyword*>();

		RE::TESObjectWEAP::Data* weaponData = (RE::TESObjectWEAP::Data*)a_weapon.instanceData.get();
		if (!weaponData) {
			return keywords;
		}

		RE::BGSKeywordForm* keywordsForm = weaponData->keywords;
		if (!keywordsForm) {
			return keywords;
		}

		for (size_t i = 0; i < keywordsForm->numKeywords; i++) {
			keywords.push_back(keywordsForm->keywords[i]);
		}

		return keywords;
	}

	// Credit: Geluxrum, Lighthouse Papyrus Extender.
	// https://github.com/GELUXRUM/LighthousePapyrusExtender/blob/main/include/Papyrus/Functions/Actor.h
	uint32_t Utility::GetWeaponLoadedAmmoCount(RE::Actor* a_this)
	{
		if (!a_this) {
			return 0;
		}

		RE::BSAutoLock lock{ a_this->currentProcess->middleHigh->equippedItemsLock };

		if (a_this->currentProcess->middleHigh->equippedItems.size() == 0) {
			return 0;
		}

		RE::EquippedItem& equippedWeapon = a_this->currentProcess->middleHigh->equippedItems[0];
		RE::TESObjectWEAP::InstanceData* weaponInstance = (RE::TESObjectWEAP::InstanceData*)equippedWeapon.item.instanceData.get();
		RE::EquippedWeaponData* weaponData = (RE::EquippedWeaponData*)equippedWeapon.data.get();

		if (equippedWeapon.equipIndex.index == 0 && weaponData && weaponInstance) {
			return weaponData->ammoCount;
		}

		return 0;
	}

	// Credit: Geluxrum, Lighthouse Papyrus Extender.
	// https://github.com/GELUXRUM/LighthousePapyrusExtender/blob/main/include/Papyrus/Functions/Actor.h
	RE::TESAmmo* Utility::GetWeaponLoadedAmmoForm(RE::Actor* a_this)
	{
		if (!a_this) {
			return nullptr;
		}

		RE::BSAutoLock lock{ a_this->currentProcess->middleHigh->equippedItemsLock };

		if (a_this->currentProcess->middleHigh->equippedItems.size() == 0) {
			return nullptr;
		}

		RE::EquippedItem& equippedWeapon = a_this->currentProcess->middleHigh->equippedItems[0];
		RE::TESObjectWEAP::InstanceData* weaponInstance = (RE::TESObjectWEAP::InstanceData*)equippedWeapon.item.instanceData.get();
		RE::EquippedWeaponData* weaponData = (RE::EquippedWeaponData*)equippedWeapon.data.get();

		if (equippedWeapon.equipIndex.index == 0 && weaponData && weaponInstance) {
			return weaponData->ammo;
		}

		return nullptr;
	}

	// Credit: Zzxyzz, RobCo Patcher.
	// https://github.com/Zzyxz/RobCo-Patcher/blob/main/utility.cpp
	template <typename T>
	T GetOffset(const void* a_baseObject, int a_offset)
	{
		return *reinterpret_cast<T*>((uintptr_t)a_baseObject + a_offset);
	}

	// Credit: Zzxyzz, RobCo Patcher.
	// https://github.com/Zzyxz/RobCo-Patcher/blob/main/utility.cpp
	RE::TESForm* Utility::GetFormFromIdentifier(const std::string& identifier)
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto delimiter = identifier.find('|');
		if (delimiter != std::string::npos) {
			std::string modName = identifier.substr(0, delimiter);
			std::string modForm = identifier.substr(delimiter + 1);

			auto* mod = dataHandler->LookupModByName(modName.c_str());
			if (mod && mod->compileIndex != -1) {
				uint32_t formID = std::stoul(modForm, nullptr, 16) & 0xFFFFFF;
				uint32_t flags = GetOffset<uint32_t>(mod, 0x334);
				if (flags & (1 << 9)) {
					// ESL
					formID &= 0xFFF;
					formID |= 0xFE << 24;
					formID |= GetOffset<uint16_t>(mod, 0x372) << 12; // ESL load order
				}
				else {
					formID |= (mod->compileIndex) << 24;
				}
				return RE::TESForm::GetFormByID(formID);
			}
		}
		return nullptr;
	}

	bool Utility::IsPluginInstalled(std::string_view a_modName)
	{
		if (a_modName.empty()) {
			return false;
		}
		return true;

		// RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();
		// if (!dataHandler) {
		// 	return false;
		// }

		// auto* modInfo = dataHandler->LookupLoadedModByName(a_modName);
		// if (modInfo) {
		// 	return true;
		// }

		// modInfo = dataHandler->LookupLoadedLightModByName(a_modName);
		// if (modInfo) {
		// 	return true;
		// }

		// return false;
	}
} // namespace Internal

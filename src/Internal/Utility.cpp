#include "Internal/Utility.hpp"
#include "Internal/Maps.hpp"
#include <string>

namespace Internal
{
	uint32_t Utility::GetWeaponDataFromMaps(const RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon)
	{
		// check keywords first, since they have priority
		std::vector<RE::BGSKeyword*> weaponKeywords = Utility::GetWeaponKeywords(a_weapon);
		if (weaponKeywords.size() > 0) {
			for (RE::BGSKeyword* keyword : weaponKeywords) {
				if (Maps::keywordMap.contains(keyword)) {
					return Maps::keywordMap.at(keyword);
				}
			}
		}

		// check weapon form second, since this is a fallback
		RE::TESObjectWEAP* weaponForm = (RE::TESObjectWEAP*)a_weapon.object;
		if (weaponForm) {
			if (Maps::weaponMap.contains(weaponForm)) {
				return Maps::weaponMap.at(weaponForm);
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

	// credit - zzxyzz robco patcher
	template <typename T>
	T GetOffset(const void* a_baseObject, int a_offset)
	{
		return *reinterpret_cast<T*>((uintptr_t)a_baseObject + a_offset);
	}

	// credit - zzxyzz robco patcher
	// Fallout4.esm|0x456 = 2
	RE::TESForm* Utility::GetFormFromIdentifier(std::string a_identifier)
	{
		if (a_identifier.empty()) {
			return nullptr;
		}

		RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			return nullptr;
		}

		size_t delimiter = a_identifier.find("|");
		if (delimiter == std::string::npos) {
			return nullptr;
		}
		std::string modName = a_identifier.substr(0, delimiter);
		std::string modForm = a_identifier.substr(delimiter + 1);
		if (!Utility::IsPluginInstalled(modName)) {
			return nullptr;
		}

		auto* modFile = dataHandler->LookupLoadedModByName(modName);
		if (modFile && modFile->compileIndex != -1) {
			uint32_t formID = std::stoul(modForm, nullptr, 16) & 0xFFFFFF;
			uint32_t flags = GetOffset<uint32_t>(modFile, 0x334);
			
			// we do this so the formid is load order agnostic
			if (flags & (1 << 9)) {
				// esl plugin
				formID &= 0xFFF;
				formID |= 0xFE << 24;
				formID |= GetOffset<uint16_t>(modFile, 0x372) << 12; // esl load order
			}
			else {
				// normal plugin
				formID |= (modFile->compileIndex) << 24;
			}
			return RE::TESForm::GetFormByID(formID);
		}

		// fail case
		return nullptr;
	}

	bool Utility::IsPluginInstalled(std::string_view a_modName)
	{
		if (a_modName.empty()) {
			return false;
		}

		RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			return false;
		}

		auto* modInfo = dataHandler->LookupLoadedModByName(a_modName);
		if (modInfo) {
			return true;
		}

		modInfo = dataHandler->LookupLoadedLightModByName(a_modName);
		if (modInfo) {
			return true;
		}

		return false;
	}
} // namespace Internal

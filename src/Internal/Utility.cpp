#include "Internal/Utility.hpp"
#include "Internal/Maps.hpp"

namespace Internal
{
	// todo - check the map from the parser for the weap's form or omods in the instance data, and set if needed
	uint32_t Utility::GetWeaponDataFromMaps(RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon)
	{
		// check keywords first since they have priority
		std::vector<RE::BGSKeyword*> weaponKeywords = GetWeaponKeywords(a_weapon);
		if (weaponKeywords.size() > 0) {
			for (RE::BGSKeyword* keyword : weaponKeywords) {
				if (Maps::keywordMap.contains(keyword)) {
					return Maps::keywordMap.at(keyword);
				}
			}
		}

		// check weapon form second since this is a fallback
		RE::TESObjectWEAP* weaponForm = (RE::TESObjectWEAP*)a_weapon.object;
		if (Maps::weaponMap.contains(weaponForm)) {
			return Maps::weaponMap.at(weaponForm);
		}

		// the data was not present, return the default amount
		return 1;
	}

	std::vector<RE::BGSKeyword*> Utility::GetWeaponKeywords(RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon)
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

} // namespace Internal

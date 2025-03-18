#include "Internal/Utility.hpp"

namespace Internal
{
	// todo - check the map from the parser for the weap's form or omods in the instance data, and set if needed
	uint32_t Utility::IsWeaponDataInMap(RE::BGSObjectInstanceT<RE::TESObjectWEAP>& a_weapon)
	{
		RE::TESObjectWEAP::Data* data = (RE::TESObjectWEAP::Data*)a_weapon.instanceData.get();
		if (!data) {
			logger::info("data was nullptr"sv);
			return 1;
		}

		RE::BSTArray<RE::EnchantmentItem*>* enchs = data->enchantments;
		if (enchs && enchs->size() > 0) {
			for (uint32_t i = 0; i < enchs->size(); i++) {
				RE::EnchantmentItem* ench = (RE::EnchantmentItem*)enchs->at(i);
				logger::info("ench on weap: {:08X}, {}"sv, ench->GetFormID(), ench->GetFormEditorID());
			}
		}
		return 1;
	}
} // namespace Internal

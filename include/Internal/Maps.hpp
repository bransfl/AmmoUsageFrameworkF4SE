#pragma once

namespace Internal
{
	class Maps
	{
	public:
		// [Form, AmmoAmount]

		static inline std::unordered_map<std::string, std::string> weaponPrepMap;
		static inline std::unordered_map<std::string, std::string> keywordPrepMap;

		static inline std::unordered_map<RE::TESObjectWEAP*, uint32_t> weaponDataMap;
		static inline std::unordered_map<RE::BGSKeyword*, uint32_t> keywordDataMap;

		/**
		 * @brief Clears all Maps to free memory.
		 */
		static inline void ClearAllMaps() noexcept
		{
			ClearPrepMaps();
			ClearDataMaps();
		}

		/**
		 * @brief Clears Prep Maps to free memory.
		 */
		static inline void ClearPrepMaps() noexcept
		{
			if (!weaponPrepMap.empty()) {
				weaponPrepMap.clear();
			}
			if (!keywordPrepMap.empty()) {
				keywordPrepMap.clear();
			}
		}

		/**
		 * @brief Clears Data Maps to free memory.
		 */
		static inline void ClearDataMaps() noexcept
		{
			if (!weaponDataMap.empty()) {
				weaponDataMap.clear();
			}
			if (!keywordDataMap.empty()) {
				keywordDataMap.clear();
			}
		}
	};
}

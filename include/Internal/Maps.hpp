#pragma once

namespace Internal
{
	class Maps
	{
	public:
		// holds the parsed data before its loaded into actual maps
		static inline std::unordered_map<std::string, std::string> weaponPrepMap;
		static inline std::unordered_map<std::string, std::string> keywordPrepMap;

		// form : ammo usage amount
		static inline std::unordered_map<RE::TESObjectWEAP*, uint32_t> weaponDataMap;
		static inline std::unordered_map<RE::BGSKeyword*, uint32_t> keywordDataMap;

		/**
		 * @brief Clears all maps to free memory.
		 */
		static inline void ClearAllMaps() noexcept
		{
			ClearPrepMaps();
			ClearDataMaps();
		}

		/**
		 * @brief Clears PrepMaps to free memory.
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
		 * @brief Clears DataMaps to free memory.
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

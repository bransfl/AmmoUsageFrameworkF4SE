#pragma once

#include <simpleini.h>

namespace Internal
{
	class Parser
	{
	public:
		/**
		 * @brief Gather and parse all inis into Maps.
		 * @details Details.
		 * @param a_ini The INI to parse.
		 */
		static void ParseINIs(CSimpleIniA& a_ini) noexcept;

		/**
		 * @brief Convert Maps::[type]PrepMap -> Maps::[type]DataMap.
		 */
		static void ParsePrepMapsToMaps() noexcept;
	};
}

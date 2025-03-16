#pragma once

#include <simpleini.h>

namespace Internal
{
	class Parser
	{
	public:
		/**
		 * @brief Gather and parse all inis.
		 * @details details
		 * @param ini ini
		 */
		static void ParseINIs(CSimpleIniA& ini) noexcept;
	};
} // namespace Internal

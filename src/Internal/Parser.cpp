#include "Internal/Parser.hpp"
#include "Internal/Maps.hpp"

#pragma warning(push)
#pragma warning(disable : 4100)

namespace Internal
{
	// todo credit music type distributor author
	void Parser::ParseINIs(CSimpleIniA& a_ini) noexcept
	{
		const std::filesystem::path filepath = R"(.\Data)";
		const std::wstring patternWStr = L"_AMMO.ini";
		const std::string patternStr = "_AMMO.ini";

		if (!exists(filepath)) {
			logger::error("ERROR: Failed to find Data directory"sv);
			return;
		}

		// TODO:
		//	collect all files
		//	loop through files
		//		parse with Utility::GetFormFromIdentifier()
	}

} // namespace Internal

#pragma warning(pop)

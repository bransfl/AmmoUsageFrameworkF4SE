#include "Internal/Parser.hpp"
#include "Internal/Maps.hpp"
#include "Internal/Utility.hpp"

namespace Internal
{
	// todo credit music type distributor author
	void Parser::ParseINIs(CSimpleIniA& a_ini) noexcept
	{
		Maps::ClearPrepMaps();
		Maps::ClearDataMaps();

		const std::filesystem::path filepath = R"(.\Data)";
		const std::wstring pattern = L"_AMMO.ini";
		constexpr std::string_view iniExtension = ".ini"sv;

		if (!exists(filepath)) {
			logger::error("Parser::ParseINIs() -> ERROR: Failed to find Data directory"sv);
			return;
		}

		CSimpleIniA::TNamesDepend sections;
		a_ini.GetAllSections(sections);

		std::vector<std::filesystem::path> ammoInis;

		// collect all _AMMO.ini files into ammoInis vector
		for (std::error_code ec{}; const auto& file : std::filesystem::directory_iterator{ filepath, ec }) {
			if (ec.value()) {
				logger::warn("ERROR CODE: {}"sv, ec.value());
				continue;
			}

			const std::filesystem::path& path = file.path();
			if (path.extension() != iniExtension) {
				continue;
			}

			const std::filesystem::path filename = path.filename();
			if (!filename.wstring().ends_with(pattern)) {
				continue;
			}

			if (std::find(ammoInis.begin(), ammoInis.end(), path) != ammoInis.end()) {
				logger::warn("Parser::ParseINIs() -> WARNING: Found duplicate _AMMO.ini file: {}"sv,
					filename.string());
				continue;
			}

			logger::info("Parser::ParseINIs() -> Found _AMMO.ini file: {}"sv, filename.string());

			ammoInis.push_back(path);
		}

		logger::info("Parser::ParseINIs() -> Finished finding _AMMO.ini files. Total count: {}"sv, ammoInis.size());

		logger::info("Parser::ParseINIs() -> Begin parsing ammoInis into Prep Map..."sv);

		// parse inis into prep map
		for (const auto& file : ammoInis) {
			const std::string filename = file.filename().string();
			logger::info("\tParser::ParseINIs() -> Loading config file: {}:"sv, filename);
			a_ini.LoadFile(file.wstring().data());
			CSimpleIniA::TNamesDepend keys{};

			// parse weapons
			if (a_ini.SectionExists("Weapons")) {
				CSimpleIniA::TNamesDepend weaponsKeys{};
				a_ini.GetAllKeys("Weapons", weaponsKeys);

				for (const auto& key : weaponsKeys) {
					if (key.pItem == nullptr) {
						logger::warn("\t\tA weapon was nullptr"sv);
						continue;
					}

					CSimpleIniA::TNamesDepend values{};
					a_ini.GetAllValues("Weapons", key.pItem, values);
					for (const auto& val : values) {
						Maps::weaponPrepMap[key.pItem] = val.pItem;
						logger::info("\t\tAdded [{}, {}] to Weapon Prep Map"sv,
							key.pItem, val.pItem);
					}
				}
			}
			else {
				logger::info("\t\tConfig file did not have a Weapons section."sv);
			}

			// parse keywords
			if (a_ini.SectionExists("Keywords")) {
				CSimpleIniA::TNamesDepend keywordsKeys{};
				a_ini.GetAllKeys("Keywords", keywordsKeys);

				for (const auto& key : keywordsKeys) {
					if (key.pItem == nullptr) {
						logger::warn("\t\tA keyword was nullptr"sv);
						continue;
					}

					CSimpleIniA::TNamesDepend values{};
					a_ini.GetAllValues("Keywords", key.pItem, values);
					for (const auto& val : values) {
						Maps::keywordPrepMap[key.pItem] = val.pItem;
						logger::info("\t\tAdded [{}, {}] to Keyword Prep Map"sv,
							key.pItem, val.pItem);
					}
				}
			}
			else {
				logger::info("\t\tConfig file did not have a Keywords section."sv);
			}

			a_ini.Reset();
		}
	}

	void Parser::ParsePrepMapsToMaps() noexcept
	{
		// parse found weapons into forms
		logger::info("Parser::ParsePrepMapsToMaps() -> Parsing Weapon Prep Map:"sv);
		if (!Maps::weaponPrepMap.empty() && Maps::weaponPrepMap.size() > 0) {
			for (const auto& weaponPrepPair : Maps::weaponPrepMap) {
				std::string weaponString = weaponPrepPair.first;
				uint32_t ammoCount = static_cast<std::uint32_t>(std::stoul(weaponPrepPair.second, nullptr, 16));

				logger::info("\tParsing: [{}, {}]"sv, weaponString, ammoCount);
				RE::TESObjectWEAP* weaponForm = (RE::TESObjectWEAP*)Utility::GetFormFromIdentifier(weaponString);
				if (weaponForm) {
					Maps::weaponDataMap.insert({ weaponForm, ammoCount });
					logger::info("\tAdded: [{}, {}]"sv, weaponString, ammoCount);
				}
				else {
					logger::info("\tSkipped: [{}, {}]"sv, weaponString, ammoCount);
				}
			}
		}

		// parse found keywords into forms
		logger::info("Parser::ParsePrepMapsToMaps() -> Parsing Keyword Prep Map:"sv);
		if (!Maps::keywordPrepMap.empty() && Maps::keywordPrepMap.size() > 0) {
			for (const auto& keywordPrepPair : Maps::keywordPrepMap) {
				std::string keywordString = keywordPrepPair.first;
				uint32_t ammoCount = static_cast<std::uint32_t>(std::stoul(keywordPrepPair.second, nullptr, 16));

				logger::info("\tParsing: [{}, {}]"sv, keywordString, ammoCount);
				RE::BGSKeyword* keywordForm = (RE::BGSKeyword*)Utility::GetFormFromIdentifier(keywordString);
				if (keywordForm) {
					Maps::keywordDataMap.insert({ keywordForm, ammoCount });
					logger::info("\tAdded: [{}, {}]"sv, keywordString, ammoCount);
				}
				else {
					logger::info("\tSkipped: [{}, {}]"sv, keywordString, ammoCount);
				}
			}
		}

		// we are finished with the prep maps, but we still need to keep data maps in memory
		Maps::ClearPrepMaps();
	}
}

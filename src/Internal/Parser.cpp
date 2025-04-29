#include "Internal/Parser.hpp"
#include "Internal/Maps.hpp"

namespace Internal
{
	// todo credit music type distributor author
	void Parser::ParseINIs(CSimpleIniA& a_ini) noexcept
	{
		const std::filesystem::path filepath = R"(.\Data)";
		const std::wstring pattern = L"_AMMO.ini";
		const std::string patternStr = "_AMMO.ini";
		const std::string_view iniExtension = ".ini"sv;

		if (!exists(filepath)) {
			logger::error("ERROR: Failed to find Data directory"sv);
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
				logger::warn("WARNING: Found duplicate _MUS.ini file: {}"sv, filename.string());
				continue;
			}

			ammoInis.push_back(path);
		}

		// parse inis into prep map
		for (const auto& f : ammoInis) {
			const std::string filename = f.filename().string();
			logger::info("Loading config file: {}", filename);
			a_ini.LoadFile(f.wstring().data());
			CSimpleIniA::TNamesDepend keys{};

			// parse weapons
			if (a_ini.SectionExists("Weapons")) {
				CSimpleIniA::TNamesDepend weaponsKeys{};
				a_ini.GetAllKeys("Weapons", weaponsKeys);

				for (const auto& k : weaponsKeys) {
					CSimpleIniA::TNamesDepend values{};
					a_ini.GetAllValues("Weapons", k.pItem, values);
					for (const auto& v : values) {
						Maps::weaponPrepMap[k.pItem] = v.pItem;
						logger::info("Added [{}: {}] to weapon prep map", k.pItem, v.pItem);
					}
				}
			}

			// parse keywords
			if (a_ini.SectionExists("Keywords")) {
				CSimpleIniA::TNamesDepend keywordsKeys{};
				a_ini.GetAllKeys("Keywords", keywordsKeys);

				for (const auto& k : keywordsKeys) {
					CSimpleIniA::TNamesDepend values{};
					a_ini.GetAllValues("Keywords", k.pItem, values);
					for (const auto& v : values) {
						Maps::keywordPrepMap[k.pItem] = v.pItem;
						logger::info("Added [{}: {}] to keyword map", k.pItem, v.pItem);
					}
				}
			}

			a_ini.Reset();
		}
	}

	void Parser::ParsePrepMapsToMaps() noexcept
	{
		// when finished
		Maps::weaponPrepMap.clear();
		Maps::keywordPrepMap.clear();
	}
} // namespace Internal

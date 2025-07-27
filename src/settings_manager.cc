#include "settings_manager.hh"
#include "log.hh"
#include "zeon.hh"
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace nlohmann;

z::SettingsManager::SettingsManager(z::Zeon* z) : zeon(z) {
}

z::SettingsManager::~SettingsManager() = default;

void z::SettingsManager::load() {
	if (std::filesystem::exists(filename)) {
		auto f = std::ifstream(filename);
		data = json::parse(f);
		f.close();

#define WEAK(expr)                                                                                 \
	try {                                                                                            \
		expr;                                                                                          \
	} catch (nlohmann::detail::type_error) {                                                         \
	}

		// load settings
		WEAK(zeon->scrollSpeed = data["scroll_speed"].template get<float>())
		WEAK(zeon->currentSearchEngine = data["search_engine"].template get<int>())
		WEAK(zeon->maxTabNameLength = data["max_tab_name_length"].template get<int>())
#undef WEAK
		INFO("z::SettingsManager::load: loaded");
	}
}

void z::SettingsManager::save() {
	data["scroll_speed"] = zeon->scrollSpeed;
	data["search_engine"] = zeon->currentSearchEngine;
	data["max_tab_name_length"] = zeon->maxTabNameLength;

	// save settings
	std::ofstream n(filename);
	n << data.dump();
	n.close();
	INFO("z::SettingsManager::save: saved");
}

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

		// load settings
		zeon->scrollSpeed = data["scroll_speed"].template get<float>();
		zeon->currentSearchEngine = data["search_engine"].template get<int>();
		INFO("z::SettingsManager::load: loaded");
	}
}

void z::SettingsManager::save() {
	data["scroll_speed"] = zeon->scrollSpeed;
	data["search_engine"] = zeon->currentSearchEngine;

	// save settings
	std::ofstream n(filename);
	n << data.dump();
	n.close();
	INFO("z::SettingsManager::save: saved");
}

#pragma once
#include "json.hpp"
#include <string>

namespace z {
class Zeon;

class SettingsManager {
public:
	inline static const std::string filename = "zeon.json";

private:
	Zeon* zeon;
	nlohmann::json data;

public:
	SettingsManager(Zeon* zeon);
	~SettingsManager();

	void load();
	void save();
};
} // namespace z

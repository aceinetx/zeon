#pragma once

#include "browser_client.hh"
#include "render_handler.hh"
#include "settings_manager.hh"
#include <cef_app.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <stdlib.h>
#include <vector>

namespace z {
extern int g_argc;
extern char** g_argv;
extern bool g_show_settings;
extern bool g_show_demo_window;

class Zeon {
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	SettingsManager settingsManager;

	bool done = false;

public:
	std::vector<CefRefPtr<CefBrowser>> browsers;
	std::vector<CefRefPtr<BrowserClient>> browserClients;
	std::vector<CefRefPtr<RenderHandler>> renderHandlers;
	CefRefPtr<CefCookieManager> cookieManager;
	int active_tab = 0;
	// Tab management
	int OpenTab(const std::string& url);
	void CloseTab(int idx);
	void SwitchTab(int idx);

	CefMainArgs cef_args;

	struct SearchEngine {
		std::string defaultUrl;
		std::string name;
	};

	std::vector<SearchEngine> searchEngines = {
			{"duckduckgo.com", "Duck duck go"},
			{"google.com", "Google"},
			{"bing.com", "Bing"},
			{"yandex.ru", "Yandex"},
	};
	int currentSearchEngine;

	enum {
		BS_LOADING,
		BS_READY,
	} BrowserState = BS_READY;

	float scrollSpeed = 5.0f;

	char ui_url[65535];

private:
	void InitAssets();

	void DrawState();
	void DrawTopBar();
	void DrawTabsBar();
	void DrawSettings();
	void Draw();

	void ProcessEvent(SDL_Event& event);
	void SetupStyle();

public:
	static std::string encodeUrlIntoGetParameter(const std::string& value);

	Zeon();
	int Init();
	void Cleanup();
	void Run();
	~Zeon();
};

extern Zeon* g_zeon;
} // namespace z

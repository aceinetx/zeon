#include "browser_client.hh"
#include "zeon.hh"
#include <cef_client.h>
#include <cef_life_span_handler.h>
#include <cef_load_handler.h>
#include <cef_render_handler.h>
#include <iostream>
#include <wrapper/cef_helpers.h>

z::BrowserClient::BrowserClient(CefRefPtr<CefRenderHandler> ptr) : handler(ptr) {
}

CefRefPtr<CefLifeSpanHandler> z::BrowserClient::GetLifeSpanHandler() {
	return this;
}

CefRefPtr<CefLoadHandler> z::BrowserClient::GetLoadHandler() {
	return this;
}

CefRefPtr<CefRenderHandler> z::BrowserClient::GetRenderHandler() {
	return handler;
}

void z::BrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	browser_id = browser->GetIdentifier();
}

bool z::BrowserClient::DoClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	if (browser->GetIdentifier() == browser_id) {
		closing = true;
	}

	return false;
}

void z::BrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
}

void z::BrowserClient::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
																 int httpStatusCode) {
	std::cout << "OnLoadEnd(" << httpStatusCode << ")" << std::endl;
	loaded = true;
	z::g_zeon->BrowserState = Zeon::BS_READY;
	strncpy(z::g_zeon->ui_url, frame->GetURL().ToString().c_str(), sizeof z::g_zeon->ui_url);
}

bool z::BrowserClient::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
																	 CefLoadHandler::ErrorCode errorCode, const CefString& failedUrl,
																	 CefString& errorText) {
	std::cout << "OnLoadError()" << std::endl;
	loaded = true;
	z::g_zeon->BrowserState = Zeon::BS_READY;
	strncpy(z::g_zeon->ui_url, frame->GetURL().ToString().c_str(), sizeof z::g_zeon->ui_url);
}

void z::BrowserClient::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading,
																						bool canGoBack, bool canGoForward) {
	std::cout << "OnLoadingStateChange()" << std::endl;
	if (isLoading) {
		z::g_zeon->BrowserState = Zeon::BS_LOADING;
	}
	strncpy(z::g_zeon->ui_url, browser->GetFocusedFrame()->GetURL().ToString().c_str(),
					sizeof z::g_zeon->ui_url);
}

void z::BrowserClient::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) {
	std::cout << "OnLoadStart()" << std::endl;
	strncpy(z::g_zeon->ui_url, frame->GetURL().ToString().c_str(), sizeof z::g_zeon->ui_url);
}

bool z::BrowserClient::closeAllowed() const {
	return closing;
}

bool z::BrowserClient::isLoaded() const {
	return loaded;
}

// Intercept popup requests to open in a new tab instead of a new window
bool z::BrowserClient::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
																		 int popup_id, const CefString& target_url,
																		 const CefString& target_frame_name,
																		 WindowOpenDisposition target_disposition, bool user_gesture,
																		 const CefPopupFeatures& popupFeatures,
																		 CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client,
																		 CefBrowserSettings& settings,
																		 CefRefPtr<CefDictionaryValue>& extra_info,
																		 bool* no_javascript_access) {
	// Only handle if a URL is provided
	if (!target_url.empty()) {
		// Open in a new tab (regardless of disposition, or restrict to
		// NEW_FOREGROUND_TAB/NEW_BACKGROUND_TAB/NEW_WINDOW/NEW_POPUP)
		z::g_zeon->OpenTab(target_url.ToString());
		return true; // Cancel default popup, we handled it
	}
	return false; // Fallback to default behavior
}

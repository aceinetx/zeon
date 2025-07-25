#include "browser_client.hh"
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

// CefLifeSpanHandler methods.
void z::BrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	// Must be executed on the UI thread.
	CEF_REQUIRE_UI_THREAD();

	browser_id = browser->GetIdentifier();
}

bool z::BrowserClient::DoClose(CefRefPtr<CefBrowser> browser) {
	// Must be executed on the UI thread.
	CEF_REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed description of this
	// process.
	if (browser->GetIdentifier() == browser_id) {
		// Set a flag to indicate that the window close should be allowed.
		closing = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}

void z::BrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
}

void z::BrowserClient::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
																 int httpStatusCode) {
	std::cout << "OnLoadEnd(" << httpStatusCode << ")" << std::endl;
	loaded = true;
}

bool z::BrowserClient::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
																	 CefLoadHandler::ErrorCode errorCode, const CefString& failedUrl,
																	 CefString& errorText) {
	std::cout << "OnLoadError()" << std::endl;
	loaded = true;
}

void z::BrowserClient::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading,
																						bool canGoBack, bool canGoForward) {
	std::cout << "OnLoadingStateChange()" << std::endl;
}

void z::BrowserClient::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) {
	std::cout << "OnLoadStart()" << std::endl;
}

bool z::BrowserClient::closeAllowed() const {
	return closing;
}

bool z::BrowserClient::isLoaded() const {
	return loaded;
}

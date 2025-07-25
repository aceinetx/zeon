#pragma once
#include <cef_app.h>

namespace z {
class BrowserClient : public CefClient, public CefLifeSpanHandler, public CefLoadHandler {
public:
	BrowserClient(CefRefPtr<CefRenderHandler> ptr);

	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;

	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override;

	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;
	void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
	bool DoClose(CefRefPtr<CefBrowser> browser) override;
	void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

	void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
								 int httpStatusCode) override;
	bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
									 CefLoadHandler::ErrorCode errorCode, const CefString& failedUrl,
									 CefString& errorText);
	void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack,
														bool canGoForward) override;
	void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame);
	bool closeAllowed() const;
	bool isLoaded() const;

private:
	int browser_id;
	bool closing = false;
	bool loaded = false;
	CefRefPtr<CefRenderHandler> handler;

	IMPLEMENT_REFCOUNTING(BrowserClient);
};
} // namespace z

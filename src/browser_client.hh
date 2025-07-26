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

	// Intercept popup requests to open in a new tab instead of a new window
	bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
						 CefRefPtr<CefFrame> frame,
						 int popup_id,
						 const CefString& target_url,
						 const CefString& target_frame_name,
						 WindowOpenDisposition target_disposition,
						 bool user_gesture,
						 const CefPopupFeatures& popupFeatures,
						 CefWindowInfo& windowInfo,
						 CefRefPtr<CefClient>& client,
						 CefBrowserSettings& settings,
						 CefRefPtr<CefDictionaryValue>& extra_info,
						 bool* no_javascript_access) override;

private:
	int browser_id;
	bool closing = false;
	bool loaded = false;
	CefRefPtr<CefRenderHandler> handler;

	IMPLEMENT_REFCOUNTING(BrowserClient);
};
} // namespace z

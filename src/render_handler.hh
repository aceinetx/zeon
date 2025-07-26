#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <cef_app.h>
#include <mutex>
#include <vulkan/vulkan.h>

namespace z {
class RenderHandler : public CefRenderHandler {
public:
	RenderHandler(SDL_Renderer* renderer, int w, int h);

	~RenderHandler();

	void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;

	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects,
							 const void* buffer, int w, int h) override;

	void resize(int w, int h);

	void render();

private:
	int width;
	int height;
	SDL_Renderer* renderer;
	SDL_Texture* texture = nullptr;
	std::recursive_mutex texture_mutex;

	IMPLEMENT_REFCOUNTING(RenderHandler);
};

} // namespace z

#include "render_handler.hh"
#include "log.hh"
#include "zeondefs.hh"
#include <iostream>

z::RenderHandler::RenderHandler(SDL_Renderer* renderer, int w, int h)
		: width(w), height(h), renderer(renderer) {
	resize(w, h);
}

z::RenderHandler::~RenderHandler() {
	if (texture) {
		SDL_DestroyTexture(texture);
	}
	renderer = nullptr;
}

void z::RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
	rect = CefRect(0, 0, width, height);
	// std::cout << "z::RenderHandler::GetViewRect: success\n";
}

void z::RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
															 const RectList& dirtyRects, const void* buffer, int w, int h) {
	if (w != width || h != height) {
		resize(w, h);
	}

	unsigned char* texture_data = nullptr;
	int texture_pitch = 0;
	size_t bufferSize = static_cast<size_t>(w) * static_cast<size_t>(h) * 4;

	std::lock_guard<std::recursive_mutex> lock(texture_mutex);
	SDL_LockTexture(texture, nullptr, (void**)&texture_data, &texture_pitch);
	memcpy(texture_data, buffer, bufferSize); // blit cef paint data onto the texture
	SDL_UnlockTexture(texture);
}

void z::RenderHandler::resize(int w, int h) {
	std::lock_guard<std::recursive_mutex> lock(texture_mutex);
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	width = w;
	height = h;
	texture =
			SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	INFO("z::RenderHandler::resize: success");
}

void z::RenderHandler::render() {
	std::lock_guard<std::recursive_mutex> lock(texture_mutex);
	SDL_FRect destRect = {0, ZEON_TOPBAR_HEIGHT, (float)width, (float)height};
	SDL_RenderTexture(renderer, texture, NULL, &destRect);
	// std::cout << "z::RenderHandler::render: success\n";
}

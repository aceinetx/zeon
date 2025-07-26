#include "render_handler.hh"
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
	std::cout << "z::RenderHandler::GetViewRect: success\n";
}

void z::RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
															 const RectList& dirtyRects, const void* buffer, int w, int h) {
	std::lock_guard<std::recursive_mutex> lock(texture_mutex);
	if (texture) {
		unsigned char* texture_data = NULL;
		int texture_pitch = 0;

		SDL_LockTexture(texture, 0, (void**)&texture_data, &texture_pitch);
		memcpy(texture_data, buffer, w * h * 4);
		SDL_UnlockTexture(texture);
		// std::cout << "z::RenderHandler::OnPaint: success\n";
	} else {
		std::cout << "z::RenderHandler::OnPaint: (texture) == false\n";
	}
}

void z::RenderHandler::resize(int w, int h) {
	std::lock_guard<std::recursive_mutex> lock(texture_mutex);
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	width = w;
	height = h;
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_STREAMING, w, h);

	std::cout << "z::RenderHandler::resize: success\n";
}

void z::RenderHandler::render() {
	std::lock_guard<std::recursive_mutex> lock(texture_mutex);
	SDL_FRect destRect = {0, ZEON_TOPBAR_HEIGHT, (float)width,
												(float)height}; // Destination rectangle
	SDL_RenderTexture(renderer, texture, NULL, &destRect);
	// std::cout << "z::RenderHandler::render: success\n";
}

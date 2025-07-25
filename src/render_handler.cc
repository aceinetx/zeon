#include "render_handler.hh"

z::RenderHandler::RenderHandler(SDL_Renderer* renderer, int w, int h)
		: width(w), height(h), renderer(renderer) {
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_STREAMING, w, h);
}

z::RenderHandler::~RenderHandler() {
	if (texture) {
		SDL_DestroyTexture(texture);
	}
	renderer = nullptr;
}

void z::RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
	rect = CefRect(0, 0, width, height);
}

void z::RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
															 const RectList& dirtyRects, const void* buffer, int w, int h) {
	if (texture) {
		unsigned char* texture_data = NULL;
		int texture_pitch = 0;

		SDL_LockTexture(texture, 0, (void**)&texture_data, &texture_pitch);
		memcpy(texture_data, buffer, w * h * 4);
		SDL_UnlockTexture(texture);
	}
}

void z::RenderHandler::resize(int w, int h) {
	if (texture) {
		SDL_DestroyTexture(texture);
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_STREAMING, w, h);
	width = w;
	height = h;
}

void z::RenderHandler::render() {
	SDL_RenderTexture(renderer, texture, NULL, NULL);
}

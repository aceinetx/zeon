#include "sdl_cef_events.hh"
#include "zeon.hh"
#include "zeondefs.hh"
#include <iostream>

static CefBrowserHost::MouseButtonType translateMouseButton(SDL_MouseButtonEvent const& e) {
	CefBrowserHost::MouseButtonType result;
	switch (e.button) {
	case SDL_BUTTON_LEFT:
	case SDL_BUTTON_X1:
		result = MBT_LEFT;
		break;

	case SDL_BUTTON_MIDDLE:
		result = MBT_MIDDLE;
		break;

	case SDL_BUTTON_RIGHT:
	case SDL_BUTTON_X2:
		result = MBT_RIGHT;
		break;
	}
	std::cout << "translateMouseButton: success\n";
	return result;
}

void z::Zeon::ProcessEvent(SDL_Event& event) {
	auto& io = ImGui::GetIO();
	ImGui_ImplSDL3_ProcessEvent(&event);

	if (event.type == SDL_EVENT_QUIT) {
		browser->GetHost()->CloseBrowser(false);
	}
	if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
			event.window.windowID == SDL_GetWindowID(window)) {
		browser->GetHost()->CloseBrowser(false);
	}
	if (event.type == SDL_EVENT_WINDOW_RESIZED) {
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		renderHandler->resize(w, h - ZEON_TOPBAR_HEIGHT);
	}

	if (!io.WantCaptureKeyboard) {
		if ((event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)) {
			handleKeyEvent(event, browser);
			std::cout << "Send key ev\n";
		}
	}
	if (!io.WantCaptureMouse) {
		if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
			CefMouseEvent cef_event;
			cef_event.x = event.button.x;
			cef_event.y = event.button.y - ZEON_TOPBAR_HEIGHT;

			browser->GetHost()->SendMouseClickEvent(cef_event, translateMouseButton(event.button), true,
																							1);
			std::cout << "Send mouse button up\n";
		}
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			CefMouseEvent cef_event;
			cef_event.x = event.button.x;
			cef_event.y = event.button.y - ZEON_TOPBAR_HEIGHT;

			browser->GetHost()->SendMouseClickEvent(cef_event, translateMouseButton(event.button), false,
																							1);
			std::cout << "Send mouse button down\n";
		}
		if (event.type == SDL_EVENT_MOUSE_MOTION) {
			CefMouseEvent cef_event;
			cef_event.x = event.motion.x;
			cef_event.y = event.motion.y - ZEON_TOPBAR_HEIGHT;

			browser->GetHost()->SendMouseMoveEvent(cef_event, false);
		}
		if (event.type == SDL_EVENT_MOUSE_WHEEL) {
			int delta_x = event.wheel.x;
			int delta_y = event.wheel.y;

			if (SDL_MOUSEWHEEL_FLIPPED == event.wheel.direction) {
				delta_y *= -1;
			} else {
				delta_x *= -1;
			}

			CefMouseEvent cef_event;
			browser->GetHost()->SendMouseWheelEvent(cef_event, delta_x * scrollSpeed * 10,
																							delta_y * scrollSpeed * 10);
		}
	}
}

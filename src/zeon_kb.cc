#include "zeon_kb.hh"
#include <cef_app.h>

int z::getKeyboardModifiers(uint16_t const mod) {
	int result = EVENTFLAG_NONE;

	if (mod & SDL_KMOD_NUM) {
		result |= EVENTFLAG_NUM_LOCK_ON;
	}

	if (mod & SDL_KMOD_CAPS) {
		result |= EVENTFLAG_CAPS_LOCK_ON;
	}

	if (mod & SDL_KMOD_CTRL) {
		result |= EVENTFLAG_CONTROL_DOWN;
	}

	if (mod & SDL_KMOD_SHIFT) {
		result |= EVENTFLAG_SHIFT_DOWN;
	}

	if (mod & SDL_KMOD_ALT) {
		result |= EVENTFLAG_ALT_DOWN;
	}
	return result;
}

int z::getWindowsKeyCode(SDL_KeyboardEvent const& key) {
	const SDL_Scancode sc = key.scancode;
	const Uint16 mod = key.mod;

	// Handle modifier keys first
	switch (sc) {
	case SDL_SCANCODE_LSHIFT:
		return 0xA0; // VK_LSHIFT
	case SDL_SCANCODE_RSHIFT:
		return 0xA1; // VK_RSHIFT
	case SDL_SCANCODE_LCTRL:
		return 0xA2; // VK_LCONTROL
	case SDL_SCANCODE_RCTRL:
		return 0xA3; // VK_RCONTROL
	case SDL_SCANCODE_LALT:
		return 0xA4; // VK_LMENU
	case SDL_SCANCODE_RALT:
		return 0xA5; // VK_RMENU (AltGr)
	default:
		break;
	}

	// Handle numpad keys with dual roles (navigation vs. numpad)
	bool useAlternate = false;
	if ((mod & SDL_KMOD_NUM) == 0) { // NumLock off
		useAlternate = true;
	} else if (mod & SDL_KMOD_SHIFT) { // Shift pressed (overrides NumLock)
		useAlternate = true;
	}

	if (useAlternate) {
		switch (sc) {
		case SDL_SCANCODE_KP_0:
			return 0x2D; // VK_INSERT
		case SDL_SCANCODE_KP_1:
			return 0x23; // VK_END
		case SDL_SCANCODE_KP_2:
			return 0x28; // VK_DOWN
		case SDL_SCANCODE_KP_3:
			return 0x22; // VK_NEXT (Page Down)
		case SDL_SCANCODE_KP_4:
			return 0x25; // VK_LEFT
		case SDL_SCANCODE_KP_5:
			return 0x0C; // VK_CLEAR
		case SDL_SCANCODE_KP_6:
			return 0x27; // VK_RIGHT
		case SDL_SCANCODE_KP_7:
			return 0x24; // VK_HOME
		case SDL_SCANCODE_KP_8:
			return 0x26; // VK_UP
		case SDL_SCANCODE_KP_9:
			return 0x21; // VK_PRIOR (Page Up)
		case SDL_SCANCODE_KP_PERIOD:
			return 0x2E; // VK_DELETE
		default:
			break;
		}
	}

	// Map all other keys
	switch (sc) {
	// Letters (A-Z, virtual keys are ASCII uppercase)
	case SDL_SCANCODE_A:
		return 'A';
	case SDL_SCANCODE_B:
		return 'B';
	case SDL_SCANCODE_C:
		return 'C';
	case SDL_SCANCODE_D:
		return 'D';
	case SDL_SCANCODE_E:
		return 'E';
	case SDL_SCANCODE_F:
		return 'F';
	case SDL_SCANCODE_G:
		return 'G';
	case SDL_SCANCODE_H:
		return 'H';
	case SDL_SCANCODE_I:
		return 'I';
	case SDL_SCANCODE_J:
		return 'J';
	case SDL_SCANCODE_K:
		return 'K';
	case SDL_SCANCODE_L:
		return 'L';
	case SDL_SCANCODE_M:
		return 'M';
	case SDL_SCANCODE_N:
		return 'N';
	case SDL_SCANCODE_O:
		return 'O';
	case SDL_SCANCODE_P:
		return 'P';
	case SDL_SCANCODE_Q:
		return 'Q';
	case SDL_SCANCODE_R:
		return 'R';
	case SDL_SCANCODE_S:
		return 'S';
	case SDL_SCANCODE_T:
		return 'T';
	case SDL_SCANCODE_U:
		return 'U';
	case SDL_SCANCODE_V:
		return 'V';
	case SDL_SCANCODE_W:
		return 'W';
	case SDL_SCANCODE_X:
		return 'X';
	case SDL_SCANCODE_Y:
		return 'Y';
	case SDL_SCANCODE_Z:
		return 'Z';

	// Top row digits
	case SDL_SCANCODE_0:
		return '0';
	case SDL_SCANCODE_1:
		return '1';
	case SDL_SCANCODE_2:
		return '2';
	case SDL_SCANCODE_3:
		return '3';
	case SDL_SCANCODE_4:
		return '4';
	case SDL_SCANCODE_5:
		return '5';
	case SDL_SCANCODE_6:
		return '6';
	case SDL_SCANCODE_7:
		return '7';
	case SDL_SCANCODE_8:
		return '8';
	case SDL_SCANCODE_9:
		return '9';

	// Function keys
	case SDL_SCANCODE_F1:
		return 0x70;
	case SDL_SCANCODE_F2:
		return 0x71;
	case SDL_SCANCODE_F3:
		return 0x72;
	case SDL_SCANCODE_F4:
		return 0x73;
	case SDL_SCANCODE_F5:
		return 0x74;
	case SDL_SCANCODE_F6:
		return 0x75;
	case SDL_SCANCODE_F7:
		return 0x76;
	case SDL_SCANCODE_F8:
		return 0x77;
	case SDL_SCANCODE_F9:
		return 0x78;
	case SDL_SCANCODE_F10:
		return 0x79;
	case SDL_SCANCODE_F11:
		return 0x7A;
	case SDL_SCANCODE_F12:
		return 0x7B;

	// Numpad keys (when not using alternate navigation)
	case SDL_SCANCODE_KP_0:
		return 0x60; // VK_NUMPAD0
	case SDL_SCANCODE_KP_1:
		return 0x61; // VK_NUMPAD1
	case SDL_SCANCODE_KP_2:
		return 0x62; // VK_NUMPAD2
	case SDL_SCANCODE_KP_3:
		return 0x63; // VK_NUMPAD3
	case SDL_SCANCODE_KP_4:
		return 0x64; // VK_NUMPAD4
	case SDL_SCANCODE_KP_5:
		return 0x65; // VK_NUMPAD5
	case SDL_SCANCODE_KP_6:
		return 0x66; // VK_NUMPAD6
	case SDL_SCANCODE_KP_7:
		return 0x67; // VK_NUMPAD7
	case SDL_SCANCODE_KP_8:
		return 0x68; // VK_NUMPAD8
	case SDL_SCANCODE_KP_9:
		return 0x69; // VK_NUMPAD9
	case SDL_SCANCODE_KP_PERIOD:
		return 0x6E; // VK_DECIMAL
	case SDL_SCANCODE_KP_PLUS:
		return 0x6B; // VK_ADD
	case SDL_SCANCODE_KP_MINUS:
		return 0x6D; // VK_SUBTRACT
	case SDL_SCANCODE_KP_MULTIPLY:
		return 0x6A; // VK_MULTIPLY
	case SDL_SCANCODE_KP_DIVIDE:
		return 0x6F; // VK_DIVIDE
	case SDL_SCANCODE_KP_ENTER:
		return 0x0D; // VK_RETURN

	// Other common keys
	case SDL_SCANCODE_RETURN:
		return 0x0D; // VK_RETURN
	case SDL_SCANCODE_ESCAPE:
		return 0x1B; // VK_ESCAPE
	case SDL_SCANCODE_BACKSPACE:
		return 0x08; // VK_BACK
	case SDL_SCANCODE_TAB:
		return 0x09; // VK_TAB
	case SDL_SCANCODE_SPACE:
		return 0x20; // VK_SPACE
	case SDL_SCANCODE_CAPSLOCK:
		return 0x14; // VK_CAPITAL
	case SDL_SCANCODE_NUMLOCKCLEAR:
		return 0x90; // VK_NUMLOCK
	case SDL_SCANCODE_SCROLLLOCK:
		return 0x91; // VK_SCROLL
	case SDL_SCANCODE_PRINTSCREEN:
		return 0x2C; // VK_SNAPSHOT
	case SDL_SCANCODE_PAUSE:
		return 0x13; // VK_PAUSE
	case SDL_SCANCODE_INSERT:
		return 0x2D; // VK_INSERT
	case SDL_SCANCODE_HOME:
		return 0x24; // VK_HOME
	case SDL_SCANCODE_END:
		return 0x23; // VK_END
	case SDL_SCANCODE_PAGEUP:
		return 0x21; // VK_PRIOR
	case SDL_SCANCODE_PAGEDOWN:
		return 0x22; // VK_NEXT
	case SDL_SCANCODE_DELETE:
		return 0x2E; // VK_DELETE
	case SDL_SCANCODE_LEFT:
		return 0x25; // VK_LEFT
	case SDL_SCANCODE_RIGHT:
		return 0x27; // VK_RIGHT
	case SDL_SCANCODE_UP:
		return 0x26; // VK_UP
	case SDL_SCANCODE_DOWN:
		return 0x28; // VK_DOWN

	// Punctuation and special keys
	case SDL_SCANCODE_GRAVE:
		return 0xC0; // VK_OEM_3
	case SDL_SCANCODE_MINUS:
		return 0xBD; // VK_OEM_MINUS
	case SDL_SCANCODE_EQUALS:
		return 0xBB; // VK_OEM_PLUS
	case SDL_SCANCODE_LEFTBRACKET:
		return 0xDB; // VK_OEM_4
	case SDL_SCANCODE_RIGHTBRACKET:
		return 0xDD; // VK_OEM_6
	case SDL_SCANCODE_BACKSLASH:
		return 0xDC; // VK_OEM_5
	case SDL_SCANCODE_SEMICOLON:
		return 0xBA; // VK_OEM_1
	case SDL_SCANCODE_APOSTROPHE:
		return 0xDE; // VK_OEM_7
	case SDL_SCANCODE_COMMA:
		return 0xBC; // VK_OEM_COMMA
	case SDL_SCANCODE_PERIOD:
		return 0xBE; // VK_OEM_PERIOD
	case SDL_SCANCODE_SLASH:
		return 0xBF; // VK_OEM_2

	// Additional keys
	case SDL_SCANCODE_MENU:
		return 0x5D; // VK_APPS
	case SDL_SCANCODE_VOLUMEUP:
		return 0xAF; // VK_VOLUME_UP
	case SDL_SCANCODE_VOLUMEDOWN:
		return 0xAE; // VK_VOLUME_DOWN
	case SDL_SCANCODE_MUTE:
		return 0xAD; // VK_VOLUME_MUTE

	default:
		return 0; // Unknown key
	}
}

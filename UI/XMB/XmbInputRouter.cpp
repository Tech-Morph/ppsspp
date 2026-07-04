#include "UI/XMB/XmbInputRouter.h"

XmbInputAction XmbInputRouter::TranslateKeyCode(int, int keyCode, bool down) const {
	if (!down)
		return XmbInputAction::None;

	// Replace these with PPSSPP/NKCODE constants in your tree.
	// Typical intent:
	// - Arrow keys or D-pad = navigation
	// - Enter / Cross / A = confirm
	// - Escape / Circle / B = back
	switch (keyCode) {
	case 19:
		return XmbInputAction::NavUp;
	case 20:
		return XmbInputAction::NavDown;
	case 21:
		return XmbInputAction::NavLeft;
	case 22:
		return XmbInputAction::NavRight;
	case 23:
	case 66:
		return XmbInputAction::Confirm;
	case 4:
	case 111:
		return XmbInputAction::Back;
	default:
		return XmbInputAction::None;
	}
}

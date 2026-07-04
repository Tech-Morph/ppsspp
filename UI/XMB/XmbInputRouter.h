#pragma once

enum class XmbInputAction {
	None,
	NavUp,
	NavDown,
	NavLeft,
	NavRight,
	Confirm,
	Back,
};

class XmbInputRouter {
public:
	XmbInputAction TranslateKeyCode(int deviceId, int keyCode, bool down) const;
};

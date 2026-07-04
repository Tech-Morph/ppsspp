#pragma once

#include <string>

class XmbScreen;
class ScreenManager;

class XmbRouter {
public:
	explicit XmbRouter(XmbScreen *screen);

	void Navigate(const std::string &route);
	void DispatchFocused();
	void DispatchRoute(const std::string &route, const std::string &payload = "");

private:
	XmbScreen *screen_ = nullptr;
};

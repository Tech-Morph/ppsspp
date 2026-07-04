#include "UI/XMB/XmbRouter.h"
#include "UI/XMB/XmbScreen.h"
#include "UI/XMB/XmbViewModel.h"

XmbRouter::XmbRouter(XmbScreen *screen) : screen_(screen) {
}

void XmbRouter::Navigate(const std::string &route) {
	if (!screen_)
		return;

	if (route == "xmb://back") {
		screen_->HandleBackAtCurrentDepth();
		return;
	}

	if (route.rfind("xmb://settings/", 0) == 0) {
		screen_->OpenSettingsPlaceholder(route);
		return;
	}

	if (route == "xmb://browser/fallback") {
		screen_->SwitchToLegacyMenu();
		return;
	}

	if (route.rfind("xmb://games/", 0) == 0) {
		screen_->OpenGamesPlaceholder(route);
		return;
	}
}

void XmbRouter::DispatchFocused() {
	if (!screen_)
		return;

	const XmbItem *item = screen_->ViewModel().FocusedItem();
	if (!item || !item->action.has_value())
		return;

	DispatchRoute(item->action->route, item->action->payload);
}

void XmbRouter::DispatchRoute(const std::string &route, const std::string &) {
	Navigate(route);
}

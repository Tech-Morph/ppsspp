#include "UI/XMB/XmbScreen.h"

#include "Common/UI/ViewGroup.h"
#include "Common/UI/View.h"
#include "UI/XMB/XmbRootPromptScreen.h"

XmbScreen::XmbScreen() : router_(this) {
}

void XmbScreen::CreateViews() {
	using namespace UI;

	// Minimal placeholder layout.
	auto *root = new LinearLayout(ORIENT_VERTICAL);

	const XmbColumn *col = viewModel_.FocusedColumn();
	root->Add(new TextView("PPSSPP XMB"));
	root->Add(new TextView(col ? col->label : "No Column"));

	if (col) {
		for (int i = 0; i < (int)col->items.size(); ++i) {
			const auto &item = col->items[i];
			std::string prefix = i == viewModel_.FocusedItemIndex() ? "> " : "  ";
			root->Add(new TextView(prefix + item.label));
		}
	}

	SetRootView(root);
}

void XmbScreen::update() {
	UIScreen::update();
}

void XmbScreen::render() {
	UIScreen::render();
}

bool XmbScreen::key(const KeyInput &key) {
	XmbInputAction action = inputRouter_.TranslateKeyCode(key.deviceId, key.keyCode, key.flags & KEY_DOWN);
	if (action != XmbInputAction::None) {
		HandleAction(action);
		return true;
	}
	return UIScreen::key(key);
}

bool XmbScreen::axis(const AxisInput &axis) {
	return UIScreen::axis(axis);
}

void XmbScreen::HandleAction(XmbInputAction action) {
	switch (action) {
	case XmbInputAction::NavUp:
		viewModel_.MoveFocus(XmbDirection::Up);
		RecreateViews();
		break;
	case XmbInputAction::NavDown:
		viewModel_.MoveFocus(XmbDirection::Down);
		RecreateViews();
		break;
	case XmbInputAction::NavLeft:
		viewModel_.MoveFocus(XmbDirection::Left);
		RecreateViews();
		break;
	case XmbInputAction::NavRight:
		viewModel_.MoveFocus(XmbDirection::Right);
		RecreateViews();
		break;
	case XmbInputAction::Confirm:
		router_.DispatchFocused();
		break;
	case XmbInputAction::Back:
		HandleBackAtCurrentDepth();
		break;
	default:
		break;
	}
}

void XmbScreen::HandleBackAtCurrentDepth() {
	if (viewModel_.IsSubmenuOpen()) {
		viewModel_.CloseSubmenu();
		RecreateViews();
		return;
	}
	ShowRootBackPrompt();
}

void XmbScreen::ShowRootBackPrompt() {
	// Replace with the exact PPSSPP screen manager push call used in your tree.
	auto callback = [this](XmbRootPromptChoice choice) {
		switch (choice) {
		case XmbRootPromptChoice::ExitPPSSPP:
			RequestExit();
			break;
		case XmbRootPromptChoice::SwitchToClassicMenu:
			SwitchToLegacyMenu();
			break;
		case XmbRootPromptChoice::Cancel:
			break;
		}
	};

	// Example placeholder. Adapt to your screen manager API.
	(void)callback;
}

void XmbScreen::SwitchToLegacyMenu() {
	// Replace with the actual legacy main screen transition.
}

void XmbScreen::RequestExit() {
	// Replace with PPSSPP's normal app exit path.
}

void XmbScreen::OpenSettingsPlaceholder(const std::string &) {
	viewModel_.OpenSubmenu();
	RecreateViews();
}

void XmbScreen::OpenGamesPlaceholder(const std::string &) {
	viewModel_.OpenSubmenu();
	RecreateViews();
}

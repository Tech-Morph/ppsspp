#include "UI/XMB/XmbRootPromptScreen.h"

#include "Common/UI/ViewGroup.h"
#include "Common/UI/View.h"

XmbRootPromptScreen::XmbRootPromptScreen(Callback callback)
	: callback_(std::move(callback)) {
}

void XmbRootPromptScreen::CreateViews() {
	using namespace UI;

	// Replace with PPSSPP's preferred layout widgets in your tree.
	// This is a stub modal screen.
	auto *root = new LinearLayout(ORIENT_VERTICAL);
	root->Add(new TextView("Exit PPSSPP?"));
	root->Add(new Button("Exit PPSSPP", [this](UI::EventParams &) {
		if (callback_) callback_(XmbRootPromptChoice::ExitPPSSPP);
		return UI::EVENT_DONE;
	}));
	root->Add(new Button("Switch to Classic Menu", [this](UI::EventParams &) {
		if (callback_) callback_(XmbRootPromptChoice::SwitchToClassicMenu);
		return UI::EVENT_DONE;
	}));
	root->Add(new Button("Cancel", [this](UI::EventParams &) {
		if (callback_) callback_(XmbRootPromptChoice::Cancel);
		return UI::EVENT_DONE;
	}));
	SetRootView(root);
}

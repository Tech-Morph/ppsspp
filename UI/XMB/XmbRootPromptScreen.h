#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Common/UI/UIScreen.h"

enum class XmbRootPromptChoice {
	ExitPPSSPP,
	SwitchToClassicMenu,
	Cancel,
};

class XmbRootPromptScreen : public UIScreen {
public:
	using Callback = std::function<void(XmbRootPromptChoice)>;

	explicit XmbRootPromptScreen(Callback callback);

	void CreateViews() override;

private:
	Callback callback_;
};

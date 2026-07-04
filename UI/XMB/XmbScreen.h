#pragma once

#include <string>

#include "Common/UI/UIScreen.h"
#include "UI/XMB/XmbInputRouter.h"
#include "UI/XMB/XmbRouter.h"
#include "UI/XMB/XmbViewModel.h"

class XmbScreen : public UIScreen {
public:
	XmbScreen();

	void CreateViews() override;
	void update() override;
	void render() override;

	// Replace these signatures with the exact PPSSPP input overrides in your tree.
	bool key(const KeyInput &key) override;
	bool axis(const AxisInput &axis) override;

	void HandleBackAtCurrentDepth();
	void ShowRootBackPrompt();
	void SwitchToLegacyMenu();
	void RequestExit();

	void OpenSettingsPlaceholder(const std::string &route);
	void OpenGamesPlaceholder(const std::string &route);

	XmbViewModel &ViewModel() { return viewModel_; }
	const XmbViewModel &ViewModel() const { return viewModel_; }

private:
	void HandleAction(XmbInputAction action);

	XmbViewModel viewModel_;
	XmbInputRouter inputRouter_;
	XmbRouter router_;
};

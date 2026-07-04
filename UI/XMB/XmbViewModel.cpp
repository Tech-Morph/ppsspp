#include "UI/XMB/XmbViewModel.h"

XmbViewModel::XmbViewModel() {
	BuildDefaultModel();
}

void XmbViewModel::BuildDefaultModel() {
	columns_.clear();

	columns_.push_back({
		XmbCategory::Settings,
		"Settings",
		"settings",
		{
			{ "settings_graphics", XmbItemType::Folder, "Graphics", "Rendering and display", "graphics", true, true, std::nullopt, XmbAction{ "xmb://settings/graphics", "" } },
			{ "settings_audio", XmbItemType::Folder, "Audio", "Volume and audio output", "audio", true, true, std::nullopt, XmbAction{ "xmb://settings/audio", "" } },
			{ "settings_controls", XmbItemType::Folder, "Controls", "Input mapping and behavior", "controls", true, true, std::nullopt, XmbAction{ "xmb://settings/controls", "" } },
			{ "settings_system", XmbItemType::Folder, "System", "General emulator behavior", "system", true, true, std::nullopt, XmbAction{ "xmb://settings/system", "" } },
			{ "settings_network", XmbItemType::Folder, "Networking", "Ad hoc and network settings", "network", true, true, std::nullopt, XmbAction{ "xmb://settings/network", "" } },
			{ "settings_tools", XmbItemType::Folder, "Tools", "Developer and utility tools", "tools", true, true, std::nullopt, XmbAction{ "xmb://settings/tools", "" } },
		}
	});

	columns_.push_back({
		XmbCategory::Game,
		"Game",
		"game",
		{
			{ "game_recent", XmbItemType::Folder, "Recent Games", "Recently played", "recent", true, true, std::nullopt, XmbAction{ "xmb://games/recent", "" } },
			{ "game_library", XmbItemType::Folder, "Library", "Browse installed games", "library", true, true, std::nullopt, XmbAction{ "xmb://games/library", "" } },
			{ "game_legacy", XmbItemType::Action, "Classic Browser", "Open legacy PPSSPP browser", "classic", true, true, std::nullopt, XmbAction{ "xmb://browser/fallback", "" } },
		}
	});

	columns_.push_back({
		XmbCategory::SaveData,
		"Save Data",
		"save",
		{
			{ "save_manage", XmbItemType::Info, "Save Data Utility", "Coming soon", "saveutil", true, true, std::nullopt, std::nullopt },
		}
	});

	columns_.push_back({
		XmbCategory::Media,
		"Media",
		"media",
		{
			{ "media_placeholder", XmbItemType::Info, "Media", "Coming soon", "media", true, true, std::nullopt, std::nullopt },
		}
	});

	columns_.push_back({
		XmbCategory::Network,
		"Network",
		"network",
		{
			{ "network_placeholder", XmbItemType::Info, "Network", "Coming soon", "network", true, true, std::nullopt, std::nullopt },
		}
	});

	columns_.push_back({
		XmbCategory::Extras,
		"Extras",
		"extras",
		{
			{ "extras_about", XmbItemType::Info, "PPSSPP", "XMB shell preview", "about", true, true, std::nullopt, std::nullopt },
		}
	});

	ResetFocus();
}

void XmbViewModel::SetColumns(std::vector<XmbColumn> columns) {
	columns_ = std::move(columns);
	ClampFocus();
}

void XmbViewModel::ResetFocus() {
	focusedColumn_ = 0;
	focusedItem_ = 0;
	submenuOpen_ = false;
	ClampFocus();
}

void XmbViewModel::MoveFocus(XmbDirection dir) {
	if (columns_.empty())
		return;

	switch (dir) {
	case XmbDirection::Left:
		focusedColumn_--;
		if (focusedColumn_ < 0)
			focusedColumn_ = (int)columns_.size() - 1;
		focusedItem_ = 0;
		break;

	case XmbDirection::Right:
		focusedColumn_++;
		if (focusedColumn_ >= (int)columns_.size())
			focusedColumn_ = 0;
		focusedItem_ = 0;
		break;

	case XmbDirection::Up: {
		const auto &items = columns_[focusedColumn_].items;
		if (items.empty())
			break;
		focusedItem_--;
		if (focusedItem_ < 0)
			focusedItem_ = (int)items.size() - 1;
		break;
	}

	case XmbDirection::Down: {
		const auto &items = columns_[focusedColumn_].items;
		if (items.empty())
			break;
		focusedItem_++;
		if (focusedItem_ >= (int)items.size())
			focusedItem_ = 0;
		break;
	}
	}

	ClampFocus();
}

const XmbColumn *XmbViewModel::FocusedColumn() const {
	if (focusedColumn_ < 0 || focusedColumn_ >= (int)columns_.size())
		return nullptr;
	return &columns_[focusedColumn_];
}

const XmbItem *XmbViewModel::FocusedItem() const {
	const XmbColumn *col = FocusedColumn();
	if (!col)
		return nullptr;
	if (focusedItem_ < 0 || focusedItem_ >= (int)col->items.size())
		return nullptr;
	return &col->items[focusedItem_];
}

void XmbViewModel::ClampFocus() {
	if (columns_.empty()) {
		focusedColumn_ = 0;
		focusedItem_ = 0;
		return;
	}

	if (focusedColumn_ < 0)
		focusedColumn_ = 0;
	if (focusedColumn_ >= (int)columns_.size())
		focusedColumn_ = (int)columns_.size() - 1;

	const auto &items = columns_[focusedColumn_].items;
	if (items.empty()) {
		focusedItem_ = 0;
		return;
	}

	if (focusedItem_ < 0)
		focusedItem_ = 0;
	if (focusedItem_ >= (int)items.size())
		focusedItem_ = (int)items.size() - 1;
}

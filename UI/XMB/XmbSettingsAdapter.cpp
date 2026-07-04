#include "UI/XMB/XmbSettingsAdapter.h"

// Later: bind these to real g_Config settings from Core/Config.
std::vector<XmbItem> XmbSettingsAdapter::BuildSection(const std::string &section) const {
	if (section == "graphics") {
		return {
			{ "gfx_backend", XmbItemType::Choice, "Backend", "Graphics backend", "graphics", true, true, XmbBinding{ "GraphicsBackend", "choice", "graphics" }, std::nullopt },
			{ "gfx_vsync", XmbItemType::Toggle, "VSync", "Vertical sync", "graphics", true, true, XmbBinding{ "VSync", "bool", "graphics" }, std::nullopt },
		};
	}
	if (section == "audio") {
		return {
			{ "audio_enable", XmbItemType::Toggle, "Enable Sound", "Master sound toggle", "audio", true, true, XmbBinding{ "EnableSound", "bool", "audio" }, std::nullopt },
		};
	}
	if (section == "controls") {
		return {
			{ "controls_map", XmbItemType::Action, "Control Mapping", "Configure controls", "controls", true, true, std::nullopt, XmbAction{ "xmb://settings/controls/map", "" } },
		};
	}
	return {
		{ "placeholder", XmbItemType::Info, "Coming soon", section, "info", true, true, std::nullopt, std::nullopt },
	};
}

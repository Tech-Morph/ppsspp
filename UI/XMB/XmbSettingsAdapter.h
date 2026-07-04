#pragma once

#include <string>
#include <vector>

#include "UI/XMB/XmbViewModel.h"

class XmbSettingsAdapter {
public:
	std::vector<XmbItem> BuildSection(const std::string &section) const;
};

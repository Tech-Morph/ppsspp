#pragma once

#include <string>
#include <vector>
#include <optional>

enum class XmbCategory {
	Settings,
	Game,
	SaveData,
	Media,
	Network,
	Extras,
};

enum class XmbItemType {
	Action,
	Folder,
	Toggle,
	Choice,
	Slider,
	Info,
};

enum class XmbDirection {
	Up,
	Down,
	Left,
	Right,
};

struct XmbBinding {
	std::string configKey;
	std::string valueType;
	std::string section;
};

struct XmbAction {
	std::string route;
	std::string payload;
};

struct XmbItem {
	std::string id;
	XmbItemType type{};
	std::string label;
	std::string subtitle;
	std::string icon;
	bool enabled = true;
	bool visible = true;
	std::optional<XmbBinding> binding;
	std::optional<XmbAction> action;
};

struct XmbColumn {
	XmbCategory category{};
	std::string label;
	std::string icon;
	std::vector<XmbItem> items;
};

class XmbViewModel {
public:
	XmbViewModel();

	void BuildDefaultModel();

	void MoveFocus(XmbDirection dir);
	void ResetFocus();

	const XmbColumn *FocusedColumn() const;
	const XmbItem *FocusedItem() const;

	int FocusedColumnIndex() const { return focusedColumn_; }
	int FocusedItemIndex() const { return focusedItem_; }

	bool IsAtRootCategory() const { return !submenuOpen_; }
	bool IsSubmenuOpen() const { return submenuOpen_; }

	void OpenSubmenu() { submenuOpen_ = true; }
	void CloseSubmenu() { submenuOpen_ = false; }

	const std::vector<XmbColumn> &Columns() const { return columns_; }
	std::vector<XmbColumn> &Columns() { return columns_; }

	void SetColumns(std::vector<XmbColumn> columns);

private:
	void ClampFocus();

	std::vector<XmbColumn> columns_;
	int focusedColumn_ = 0;
	int focusedItem_ = 0;
	bool submenuOpen_ = false;
};

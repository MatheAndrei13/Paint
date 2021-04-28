#pragma once
#include "Texture.h"
#include <string>

class Paint;

class ImagePanel {
	std::wstring name;
	Texture texture;
	Color color;

	Texture newButton;
	Texture saveButton;
	Texture loadButton;
	Texture exitButton;
	Texture shortcutButton;

public:
	ImagePanel();

	bool inBounds(Vec2 vec2) const;

	void Init(std::wstring name, Vec2 size, Rect region, Color color);
	void Update(Paint& app);

private:
	void DrawButtons();

public:
	void PressButton(Paint& app, Vec2 mousePosition);
};

class ShortcutsPanel {
	std::wstring name;
	Texture texture;
	Color color;

	Texture newShortcutLabel;
	Texture saveShortcutLabel;
	Texture loadShortcutLabel;
	Texture exitShortcutLabel;
	Texture increaseFontSizeShortcutLabel;
	Texture decreaseFontSizeShortcutLabel;
	Texture nextFGColorShortcutLabel;
	Texture prevFGColorShortcutLabel;
	Texture nextBGColorShortcutLabel;
	Texture prevBGColorShortcutLabel;
	Texture undoShortcutLabel;
	Texture redoShortcutLabel;
	Texture pencilShortcutLabel;
	Texture eraserShortcutLabel;
	Texture lineShortcutLabel;
	Texture rectangleShortcutLabel;
	Texture ellipseShortcutLabel;
	Texture bucketShortcutLabel;
	Texture pickerShortcutLabel;
	Texture copyShortcutLabel;
	Texture pasteShortcutLabel;
	Texture clearShortcutLabel;

public:
	bool active;

public:
	ShortcutsPanel();

	bool inBounds(Vec2 vec2) const;

	void Init(std::wstring name, Vec2 size, Rect region, Color color);
	void Update(Paint& app);

private:
	void DrawShortcutLabels();

public:
	void Show();
	void Hide(Paint& app);
};

class InfoPanel {
	std::wstring name;
	Texture texture;
	Color color;

	Texture previewLabel;
	Texture glyphLabel;
	Texture fgColorLabel;
	Texture bgColorLabel;
	Texture imageLabel;
	Texture imageNameLabel;

public:
	InfoPanel();

	bool inBounds(Vec2 vec2) const;

	void Init(std::wstring name, Vec2 size, Rect region, Color color, Glyph glyph, Color fgColor, Color bgColor, std::string imageName);
	void Update(Paint& app);

private:
	void DrawLabels();

public:
	void ChangeGlyph(Glyph glyph);
	void ChangeFGColor(Color color);
	void ChangeBGColor(Color color);
	void ChangeImageName(std::string imageName);
};

class ToolPanel {
	std::wstring name;
	Texture texture;
	Color color;

	TOOL selectedTool;

	Texture pencilButton;
	Texture eraserButton;
	Texture lineButton;
	Texture rectangleButton;
	Texture ellipseButton;
	Texture bucketButton;
	Texture pickerButton;
	Texture copyButton;
	Texture pasteButton;
	Texture undoButton;
	Texture redoButton;
	Texture clearButton;

public:
	ToolPanel();

	bool inBounds(Vec2 vec2) const;

	void Init(std::wstring name, Vec2 size, Rect region, Color color, TOOL tool);
	void Update(Paint& app);

private:
	void DrawButtons();

public:
	void ChangeTool(TOOL tool);

	void SelectTool(Paint& app, Vec2 mousePosition);
	void PressButton(Paint& app, Vec2 mousePosition);
};

class GlyphPanel {
	std::wstring name;
	Texture texture;
	Color color;

public:
	GlyphPanel();

	bool inBounds(Vec2 vec2) const;

	void Init(std::wstring name, Vec2 size, Rect region, Color color, Glyph glyph);
	void Update(Paint& app);

private:
	void DrawGlyphs(short pageNumber);

public:
	void ChangeGlyph(Glyph glyph);

	void SelectGlyph(Paint& app, Vec2 mousePosition);
	void SelectPage(Vec2 mousePosition, Glyph glyph);
};

class PalettePanel {
	std::wstring name;
	Texture texture;
	Color color;

public:
	PalettePanel();

	bool inBounds(Vec2 vec2) const;

	void Init(std::wstring name, Vec2 size, Rect region, Color color, Color fgColor, Color bgColor);
	void Update(Paint& app);

private:
	void DrawPalette();

public:
	void ChangeFGColor(Color color);
	void ChangeBGColor(Color color);

	void SelectColor(Paint& app, Vec2 mousePosition);
};

/* ####### INPUT FIELD ####### */
struct InputField {
	Texture texture;
	std::string value;
	Color color;

	Glyph cursorGlyph;
	short cursorPosition;
	
	short maxNumOfCharacters;

	InputField();

	void Init(Vec2 size, Rect region, Color color);

	void Update(Paint& app);

	void Reset();

	void CheckInput(Paint& app);
};

class InputPanel {
	std::wstring name;
	Texture texture;
	Color color;

	InputField inputField;

public:
	std::string value;

	bool active;

public:
	InputPanel();

	bool inBounds(Vec2 vec2) const;

	void Init(std::wstring name, Vec2 size, Rect region, Color color);
	void Update(Paint& app);

	void Enable();
	void Disable();

	void CheckInput(Paint& app);
};
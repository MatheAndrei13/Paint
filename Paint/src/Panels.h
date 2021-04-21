#pragma once
#include "Texture.h"
#include <string>

class Paint;

class ImagePanel {
	std::wstring name;
	Texture texture;
	Color color;

	Texture newButton;
	Texture resizeButton;
	Texture saveButton;
	Texture loadButton;
	Texture exportButton;
	Texture exitButton;

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

class InfoPanel {
	std::wstring name;
	Texture texture;
	Color color;

	Texture previewLabel;
	Texture glyphLabel;
	Texture fgColorLabel;
	Texture bgColorLabel;

public:
	InfoPanel();

	bool inBounds(Vec2 vec2) const;

	void Init(std::wstring name, Vec2 size, Rect region, Color color, Glyph glyph, Color fgColor, Color bgColor);
	void Update(Paint& app);

private:
	void DrawLabels();

public:
	void ChangeGlyph(Glyph glyph);
	void ChangeFGColor(Color color);
	void ChangeBGColor(Color color);
};

class ToolPanel {
	std::wstring name;
	Texture texture;
	Color color;

	TOOL selectedTool;

	Texture pencilButton;
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
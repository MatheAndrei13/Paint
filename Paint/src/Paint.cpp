#include "Paint.h"

Paint::Paint() {
	canvas = nullptr;
	canvasSize = Vec2(0, 0);
	canvasRegion = { 0, 0, 0, 0 };

	glyph = NULL;
	fgColor = NULL;
	bgColor = NULL;

	tool = TOOL::Pencil;
}

Paint::~Paint() {
	if (canvas != nullptr) {
		delete[] canvas;
		canvas = nullptr;
	}
}

void Paint::DrawVisualElements() {
	if (updateCanvas) {
		DrawBuffer(canvas, { (short)canvasSize.x, (short)canvasSize.y }, canvasRegion);
		updateCanvas = false;
	}
	if (panel.update) {
		DrawBuffer(panel.buffer, { (short)panel.size.x, (short)panel.size.y }, panel.region);
		panel.update = false;
	}
}

void Paint::ChangeGlyph(wchar_t glyph) {
	this->glyph = glyph;

	panel.buffer[panel.previewTextPos.x + panel.previewText.length() + panel.size.x * panel.previewTextPos.y].Char.UnicodeChar = glyph;
	panel.update = true;
}
void Paint::ChangeFGColor(Color color) {
	fgColor = color;

	panel.buffer[panel.previewTextPos.x + panel.previewText.length() + panel.size.x * panel.previewTextPos.y].Attributes = bgColor | fgColor;
	panel.buffer[panel.fgColorTextPos.x + panel.fgColorText.length() + panel.size.x * panel.fgColorTextPos.y].Attributes = fgColor << 4;
	panel.update = true;
}
void Paint::ChangeBGColor(Color color) {
	bgColor = color;

	panel.buffer[panel.previewTextPos.x + panel.previewText.length() + panel.size.x * panel.previewTextPos.y].Attributes = bgColor | fgColor;
	panel.buffer[panel.bgColorTextPos.x + panel.bgColorText.length() + panel.size.x * panel.bgColorTextPos.y].Attributes = bgColor;
	panel.update = true;
}
void Paint::ChangeTool(TOOL tool) {
	this->tool = tool;

	for (int i = 0; i < (int)panel.toolName.length(); ++i) {
		panel.buffer[i + panel.toolTextPos.x + panel.toolText.length() + panel.size.x * panel.toolTextPos.y].Char.UnicodeChar = NULL;
		panel.buffer[i + panel.toolTextPos.x + panel.toolText.length() + panel.size.x * panel.toolTextPos.y].Attributes = 0x0070 | 0x0000;
	}

	if (tool == TOOL::Pencil)
		panel.toolName = L"Pencil";
	else if (tool == TOOL::Line)
		panel.toolName = L"Line";
	else
		panel.toolName = L"Rectangle";

	for (int i = 0; i < (int)panel.toolName.length(); ++i) {
		panel.buffer[i + panel.toolTextPos.x + panel.toolText.length() + panel.size.x * panel.toolTextPos.y].Char.UnicodeChar = panel.toolName.at(i);
		panel.buffer[i + panel.toolTextPos.x + panel.toolText.length() + panel.size.x * panel.toolTextPos.y].Attributes = 0x0070 | 0x0000;
	}

	panel.update = true;
}

void Paint::NextFGColor() {
	if (fgColor == 0x000F)
		fgColor = 0x0000;
	else
		fgColor++;

	panel.buffer[panel.previewTextPos.x + panel.previewText.length() + panel.size.x * panel.previewTextPos.y].Attributes = bgColor | fgColor;
	panel.buffer[panel.fgColorTextPos.x + panel.fgColorText.length() + panel.size.x * panel.fgColorTextPos.y].Attributes = fgColor << 4;
	panel.update = true;
}
void Paint::PreviousFGColor() {
	if (fgColor == 0x0000)
		fgColor = 0x000F;
	else
		fgColor--;

	panel.buffer[panel.previewTextPos.x + panel.previewText.length() + panel.size.x * panel.previewTextPos.y].Attributes = bgColor | fgColor;
	panel.buffer[panel.fgColorTextPos.x + panel.fgColorText.length() + panel.size.x * panel.fgColorTextPos.y].Attributes = fgColor << 4;
	panel.update = true;
}
void Paint::NextBGColor() {
	if (bgColor == 0x0000)
		bgColor = 0x0010;
	else if (bgColor == 0x00F0)
		bgColor = 0x0000;
	else
		bgColor += 0x0010;

	panel.buffer[panel.previewTextPos.x + panel.previewText.length() + panel.size.x * panel.previewTextPos.y].Attributes = bgColor | fgColor;
	panel.buffer[panel.bgColorTextPos.x + panel.bgColorText.length() + panel.size.x * panel.bgColorTextPos.y].Attributes = bgColor;
	panel.update = true;
}
void Paint::PreviousBGColor() {
	if (bgColor == 0x0000)
		bgColor = 0x00F0;
	else if (bgColor == 0x0010)
		bgColor = 0x0000;
	else
		bgColor -= 0x0010;

	panel.buffer[panel.previewTextPos.x + panel.previewText.length() + panel.size.x * panel.previewTextPos.y].Attributes = bgColor | fgColor;
	panel.buffer[panel.bgColorTextPos.x + panel.bgColorText.length() + panel.size.x * panel.bgColorTextPos.y].Attributes = bgColor;
	panel.update = true;
}

bool Paint::inCanvas(int x, int y) const { return (x >= 0 && y >= 0) && (x < canvasSize.x&& y < canvasSize.y); }
bool Paint::inCanvas(Vec2 vec2) const { return (vec2.x >= 0 && vec2.y >= 0) && (vec2.x < canvasSize.x&& vec2.y < canvasSize.y); }

void Paint::Draw(int x, int y, wchar_t glyph, Color color) {
	if (!inCanvas(x, y))
		return;

	canvas[x + canvasSize.x * y].Char.UnicodeChar = glyph;
	canvas[x + canvasSize.x * y].Attributes = color;

	updateCanvas = true;
}
void Paint::Draw(Vec2 vec2, wchar_t glyph, Color color) {
	if (!inCanvas(vec2))
		return;

	canvas[vec2.x + canvasSize.x * vec2.y].Char.UnicodeChar = glyph;
	canvas[vec2.x + canvasSize.x * vec2.y].Attributes = color;

	updateCanvas = true;
}

void Paint::Line(Vec2 start, Vec2 end, wchar_t glyph, Color color) {
	int dx = abs(end.x - start.x);
	int dy = -abs(end.y - start.y);

	int x_step = (start.x < end.x) ? 1 : -1;
	int y_step = (start.y < end.y) ? 1 : -1;

	int x = start.x;
	int y = start.y;

	int err = dx + dy;
	while (true) {
		Draw(x, y, glyph, color);
		if (x == end.x && y == end.y)
			break;
		int e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x += x_step;
		}
		if (e2 <= dx) {
			err += dx;
			y += y_step;
		}
	}
}
void Paint::GhostLine(Vec2 start, Vec2 end, wchar_t glyph, Color color) {
	int dx = abs(end.x - start.x);
	int dy = -abs(end.y - start.y);

	int x_step = (start.x < end.x) ? 1 : -1;
	int y_step = (start.y < end.y) ? 1 : -1;

	int x = start.x;
	int y = start.y;

	int err = dx + dy;
	while (true) {
		if (inCanvas(x, y))
			SetPixel(x, y, glyph, color);
		if (x == end.x && y == end.y)
			break;
		int e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x += x_step;
		}
		if (e2 <= dx) {
			err += dx;
			y += y_step;
		}
	}

	updateCanvas = true;
}

void Paint::Rectangle(Vec2 start, Vec2 end, wchar_t glyph, Color color) {
	if (start.x <= end.x && start.y > end.y) {
		int aux = start.y;
		start.y = end.y;
		end.y = aux;
	}
	else if (start.x > end.x && start.y <= end.y) {
		int aux = start.x;
		start.x = end.x;
		end.x = aux;
	}
	else if (start.x > end.x && start.y > end.y) {
		Vec2 aux = start;
		start = end;
		end = aux;
	}

	for (int x = start.x; x <= end.x; ++x) {
		Draw(x, start.y, glyph, color);
		Draw(x, end.y, glyph, color);
	}
	for (int y = start.y + 1; y < end.y; ++y) {
		Draw(start.x, y, glyph, color);
		Draw(end.x, y, glyph, color);
	}
}
void Paint::GhostRectangle(Vec2 start, Vec2 end, wchar_t glyph, Color color) {
	if (start.x <= end.x && start.y > end.y) {
		int aux = start.y;
		start.y = end.y;
		end.y = aux;
	}
	else if (start.x > end.x && start.y <= end.y) {
		int aux = start.x;
		start.x = end.x;
		end.x = aux;
	}
	else if (start.x > end.x && start.y > end.y) {
		Vec2 aux = start;
		start = end;
		end = aux;
	}

	for (int x = start.x; x <= end.x; ++x) {
		if (inCanvas(x, start.y))
			SetPixel(x, start.y, glyph, color);
		if (inCanvas(x, end.y))
			SetPixel(x, end.y, glyph, color);
	}
	for (int y = start.y + 1; y < end.y; ++y) {
		if (inCanvas(start.x, y))
			SetPixel(start.x, y, glyph, color);
		if (inCanvas(end.x, y))
			SetPixel(end.x, y, glyph, color);
	}

	updateCanvas = true;
}


void Paint::ClearCanvas() {
	for (int x = 0; x < canvasSize.x; ++x)
		for (int y = 0; y < canvasSize.y; ++y)
			Draw(x, y, NULL, 0x0000);
}


/* ####### PANEL ####### */
Paint::Panel::Panel() {
	buffer = nullptr;
	size = Vec2(0, 0);
	region = { 0, 0, 0, 0 };
	color = NULL;
}

Paint::Panel::~Panel() {
	if (buffer != nullptr) {
		delete[] buffer;
		buffer = nullptr;
	}
}

void Paint::Panel::DrawPanel(wchar_t glyph, Color fgColor, Color bgColor, TOOL tool) {
	// Panel base color
	for (int x = 0; x < size.x; ++x)
		for (int y = 0; y < size.y; ++y) {
			buffer[x + size.x * y].Char.UnicodeChar = NULL;
			buffer[x + size.x * y].Attributes = color;
		}

	// Preview text
	for (int i = 0; i < (int)previewText.length(); ++i) {
		buffer[i + previewTextPos.x + size.x * previewTextPos.y].Char.UnicodeChar = previewText.at(i);
		buffer[i + previewTextPos.x + size.x * previewTextPos.y].Attributes = 0x0070 | 0x0000;
	}
	buffer[previewTextPos.x + previewText.length() + size.x * previewTextPos.y].Char.UnicodeChar = glyph;
	buffer[previewTextPos.x + previewText.length() + size.x * previewTextPos.y].Attributes = bgColor | fgColor;

	// Foreground text
	for (int i = 0; i < (int)fgColorText.length(); ++i) {
		buffer[i + fgColorTextPos.x + size.x * fgColorTextPos.y].Char.UnicodeChar = fgColorText.at(i);
		buffer[i + fgColorTextPos.x + size.x * fgColorTextPos.y].Attributes = 0x0070 | 0x0000;
	}
	buffer[fgColorTextPos.x + fgColorText.length() + size.x * fgColorTextPos.y].Char.UnicodeChar = NULL;
	buffer[fgColorTextPos.x + fgColorText.length() + size.x * fgColorTextPos.y].Attributes = fgColor << 4;

	// Background text
	for (int i = 0; i < (int)bgColorText.length(); ++i) {
		buffer[i + bgColorTextPos.x + size.x * bgColorTextPos.y].Char.UnicodeChar = bgColorText.at(i);
		buffer[i + bgColorTextPos.x + size.x * bgColorTextPos.y].Attributes = 0x0070 | 0x0000;
	}
	buffer[bgColorTextPos.x + bgColorText.length() + size.x * bgColorTextPos.y].Char.UnicodeChar = NULL;
	buffer[bgColorTextPos.x + bgColorText.length() + size.x * bgColorTextPos.y].Attributes = bgColor;

	// Tool Text
	for (int i = 0; i < (int)toolText.length(); ++i) {
		buffer[i + toolTextPos.x + size.x * toolTextPos.y].Char.UnicodeChar = toolText.at(i);
		buffer[i + toolTextPos.x + size.x * toolTextPos.y].Attributes = 0x0070 | 0x0000;
	}
	
	if (tool == TOOL::Pencil)
		toolName = L"Pencil";
	else if (tool == TOOL::Line)
		toolName = L"Line";
	else
		toolName = L"Rectangle";

	for (int i = 0; i < (int)toolName.length(); ++i) {
		buffer[i + toolTextPos.x + toolText.length() + size.x * toolTextPos.y].Char.UnicodeChar = toolName.at(i);
		buffer[i + toolTextPos.x + toolText.length() + size.x * toolTextPos.y].Attributes = 0x0070 | 0x0000;
	}
}
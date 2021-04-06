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

void Paint::DrawCanvas() {
	if (updateCanvas) {
		DrawBuffer(canvas, { (short)canvasSize.x, (short)canvasSize.y }, canvasRegion);
		updateCanvas = false;
	}
}

void Paint::ChangeGlyph(wchar_t glyph) { this->glyph = glyph; }
void Paint::ChangeFGColor(Color color) { fgColor = color; }
void Paint::ChangeBGColor(Color color) { bgColor = color; }

void Paint::SwitchFGColor() {
	if (++fgColor > 0x000F)
		fgColor = 0;
}
void Paint::SwitchBGColor() {
	if (bgColor == 0x0000)
		bgColor = 0x0010;
	else
		bgColor += 0x0010;

	if (bgColor > 0x00F0)
		bgColor = 0x0000;
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
		SetPixel(x, start.y, glyph, color);
		SetPixel(x, end.y, glyph, color);
	}
	for (int y = start.y + 1; y < end.y; ++y) {
		SetPixel(start.x, y, glyph, color);
		SetPixel(end.x, y, glyph, color);
	}

	updateCanvas = true;
}


void Paint::ClearCanvas() {
	for (int x = 0; x < canvasSize.x; ++x)
		for (int y = 0; y < canvasSize.y; ++y)
			Draw(x, y, NULL, 0x0000);
}
#include "Texture.h"

Texture::Texture() {
	buffer = nullptr;
	size = Vec2(0, 0);
	region = Rect(0, 0, 0, 0);
	update = false;
}

Texture::~Texture() {
	Reset();
}

bool Texture::inBounds(Vec2 vec2) const {
	return (vec2.x >= region.left && vec2.y >= region.top) && (vec2.x <= region.right && vec2.y <= region.bottom);
}

void Texture::Init(Vec2 size, Rect region) {
	buffer = new CHAR_INFO[size.x * size.y];
	this->size = size;
	this->region = region;
	update = true;

	Clear();
}

void Texture::Reset() {
	if (buffer != nullptr) {
		delete[] buffer;
		buffer = nullptr;
	}

	size = Vec2(0, 0);
	region = Rect(0, 0, 0, 0);
	update = false;
}

void Texture::SetPixel(Vec2 vec2, Glyph glyph, Color color) {
	if (!((vec2.x >= 0 && vec2.y >= 0) && (vec2.x <= size.x - 1 && vec2.y <= size.y - 1)))
		return;

	buffer[vec2.x + size.x * vec2.y].Char.UnicodeChar = glyph;
	buffer[vec2.x + size.x * vec2.y].Attributes = color;

	update = true;
}

CHAR_INFO Texture::GetPixel(Vec2 vec2) const {
	return buffer[vec2.x + size.x * vec2.y];
}

void Texture::Clear() {
	for (short x = 0; x < size.x; ++x)
		for (short y = 0; y < size.y; ++y)
			SetPixel(Vec2(x, y), EMPTY_GLYPH, 0x0000);
}
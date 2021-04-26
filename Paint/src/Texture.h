#pragma once
#include <Windows.h>
#include "Types.h"

struct Texture {
	CHAR_INFO* buffer;
	Vec2 size;
	Rect region;

	bool update;

	Texture();
	~Texture();

	bool inBounds(Vec2 vec2) const;

	void Init(Vec2 size, Rect region);
	void Reset();

	void SetPixel(Vec2 vec2, Glyph glyph, Color color);
	CHAR_INFO GetPixel(Vec2 vec2) const;

	void Clear();
};


#pragma once
#include "../include/MathUtils.h"

#define EMPTY_GLYPH L' '

using Glyph = wchar_t;
using Color = unsigned char;

enum class TOOL {
	Pencil, Line, Rectangle, Ellipse, Bucket, Picker
};
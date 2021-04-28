#pragma once
#include "../include/MathUtils.h"

#define EMPTY_GLYPH L' '

using Glyph = wchar_t;
using Color = unsigned char;

enum class TOOL {
	Pencil, Eraser, Line, Rectangle, Ellipse, Bucket, Picker, Copy, Paste
};
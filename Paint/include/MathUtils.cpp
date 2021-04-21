#include "MathUtils.h"

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(short x, short y) : x(x), y(y) {}

bool Vec2::operator==(const Vec2& vec2) const {
	return x == vec2.x && y == vec2.y;
}
bool Vec2::operator!=(const Vec2& vec2) const {
	return x != vec2.x || y != vec2.y;
}

const Vec2 Vec2::operator+(const Vec2& vec2) const {
	return Vec2(x + vec2.x, y + vec2.y);
}
const Vec2 Vec2::operator-(const Vec2& vec2) const {
	return Vec2(x - vec2.x, y - vec2.y);
}
const Vec2 Vec2::operator*(const short& var) const {
	return Vec2(x * var, y * var);
}

void Vec2::operator+=(const Vec2& vec2) {
	x = x + vec2.x;
	y = y + vec2.y;
}
void Vec2::operator-=(const Vec2& vec2) {
	x = x - vec2.x;
	y = y - vec2.y;
}
void Vec2::operator*=(const short& var) {
	x = x * var;
	y = y * var;
}

Rect::Rect() : left(0), top(0), right(0), bottom(0) {}
Rect::Rect(short left, short top, short right, short bottom) : left(left), top(top), right(right), bottom(bottom) {}

bool Rect::operator==(const Rect& rect) const {
	return left == rect.left && top == rect.top && right == rect.right && bottom == rect.bottom;
}
bool Rect::operator!=(const Rect& rect) const {
	return left != rect.left || top != rect.top || right != rect.right || bottom != rect.bottom;
}
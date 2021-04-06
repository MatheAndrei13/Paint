#include "MathUtils.h"

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(int x, int y) : x(x), y(y) {}

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
const Vec2 Vec2::operator*(const int& var) const {
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
void Vec2::operator*=(const int& var) {
	x = x * var;
	y = y * var;
}
#include "Paint.h"
#include "Texture.h"


int main() {
	COORD appSize = { 120, 60 };
	COORD fontSize = { 8, 8 };

	AddFontResourceA("D:/User/MyProjects/Visual Studio Projects/Paint/Paint/resources/fonts/Square Mono.ttf");

	Paint paint;
	paint.Create(L"Paint", appSize.X, appSize.Y, L"Square Mono", fontSize.X, fontSize.Y);
	paint.Start();

	while(RemoveFontResourceA("D:/User/MyProjects/Visual Studio Projects/Paint/Paint/resources/fonts/Square Mono.ttf"));

	return 0;
}
#include "Paint.h"
#include <fstream>

int main() {
	Paint paint;
	paint.Create(L"Paint", 120, 30, 8, 16);
	paint.Start();
	return 0;
}
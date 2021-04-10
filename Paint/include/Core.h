#pragma once
#include <Windows.h>
#include <string>
#include "MathUtils.h"

class ConsoleApp {
private:
	std::wstring appName;
	HANDLE handleIn;
	HANDLE handleOut;
	SMALL_RECT windowSize;
	COORD screenSize;

	CHAR_INFO* screenBuffer;
	bool updateBuffer = false;

	bool invalidConsole = true;
	bool running = false;

	struct Key {
		bool oldState = false;
		bool newState = false;
	};

	struct MouseInput {
		static constexpr short numButtons = 2;
		COORD position = { 0, 0 };
		Key buttons[numButtons];
	} mouseInput;

	struct KeyboardInput {
		static constexpr short numKeys = 256;
		Key keys[numKeys];
	} keyboardInput;

public:
	ConsoleApp();
	void Create(std::wstring appName, int screenWidth, int screenHeight, std::wstring fontFace, int fontWidth, int fontHeight);
	~ConsoleApp();

	void Start();
	void Close();

private:
	void Run();
	void GetInput();
	void GetMouseInput(INPUT_RECORD* inputBuffer, DWORD numEvents);
	void GetKeyboardInput();

	void ResetConsole();
	void Error(const wchar_t* message);

protected:
	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;

public:
	Vec2 GetMousePosition() const;
	bool KeyPressed(const unsigned char) const;
	bool KeyReleased(const unsigned char) const;
	bool KeyHeld(const unsigned char) const;

	// TOOLS
public:
	int GetScreenWidth() const;
	int GetScreenHeight() const;

	bool OutOfBounds(int x, int y) const;

	void SetPixel(int x, int y, wchar_t glyph, unsigned char color);
	void SetPixel(Vec2 vec2, wchar_t glyph, unsigned char color);
	void Fill(int x0, int y0, int x1, int y1, wchar_t glyph, unsigned char color);
	void Fill(Vec2 vec2_0, Vec2 vec2_1, wchar_t glyph, unsigned char color);

	void DrawBuffer(CHAR_INFO* buffer, COORD bufferSize, SMALL_RECT region);

	void ClearScreen();
};
#include "Core.h"
#include <thread>
#include <chrono>

ConsoleApp::ConsoleApp() {
	// Set Console Title
	appName = L"Default";
	SetConsoleTitle(appName.c_str());

	// Get Handles
	handleIn = GetStdHandle(STD_INPUT_HANDLE);
	handleOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// Init Screen Buffer & Window Size
	screenSize = { 120, 9001 };
	windowSize = { 0, 0, 119, 29 };

	screenBuffer = nullptr;
}

void ConsoleApp::Create(std::wstring appName, int screenWidth, int screenHeight, int fontWidth, int fontHeight) {
	// Set App Title
	this->appName = appName;
	SetConsoleTitle(appName.c_str());

	// Reset Window Size to Minimum
	windowSize = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(handleOut, TRUE, &windowSize);

	// Set Screen Buffer Size
	screenSize = { (short)screenWidth, (short)screenHeight };
	if (!SetConsoleScreenBufferSize(handleOut, screenSize)) {
		Error(L"Error: SetConsoleScreenBufferSize");
		return;
	}
	/*if (!SetConsoleActiveScreenBuffer(handleOut)) {
		Error(L"Error: SetConsoleActiveScreenBuffer");
		return;
	}*/

	// Set Font Size
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize = COORD({ (short)fontWidth, (short)fontHeight });
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;
	wcscpy_s(fontInfo.FaceName, L"Consolas");
	if (!SetCurrentConsoleFontEx(handleOut, FALSE, &fontInfo)) {
		Error(L"Error: SetCurrentConsoleFontEx");
		return;
	}

	// Check if Screen Buffer Size exceeded the Maximim Allowed Window Size
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	if (!GetConsoleScreenBufferInfo(handleOut, &screenBufferInfo)) {
		Error(L"Error: GetConsoleScreenBufferInfo");
		return;
	}
	if ((short)screenWidth > screenBufferInfo.dwMaximumWindowSize.X) {
		Error(L"Error: Screen Width Exceeded");
		return;
	}
	if ((short)screenHeight > screenBufferInfo.dwMaximumWindowSize.Y) {
		Error(L"Error: Screen Height Exceeded");
		return;
	}

	// Set Window Size
	windowSize = { 0, 0, (short)screenWidth - 1, (short)screenHeight - 1 };
	if (!SetConsoleWindowInfo(handleOut, TRUE, &windowSize)) {
		Error(L"Error: SetConsoleWindowInfo");
		return;
	}

	// Disable Mouse Edit Mode & Enable Mouse Input
	if (!SetConsoleMode(handleIn, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT)) {
		Error(L"Error: SetConsoleMode");
		return;
	}

	// Hide Console Cursor
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 25;
	cursorInfo.bVisible = FALSE;
	if (!SetConsoleCursorInfo(handleOut, &cursorInfo)) {
		Error(L"Error: SetConsoleCursorInfo");
		return;
	}

	// Disable Maximizing & Resizing Window
	HWND consoleWindow = GetConsoleWindow();
	LONG windowStyle = GetWindowLong(consoleWindow, GWL_STYLE);
	SetWindowLong(consoleWindow, GWL_STYLE, windowStyle & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);


	// Allocate Memory for Screen Buffer
	screenBuffer = new CHAR_INFO[screenWidth * screenHeight];
	ClearScreen();

	invalidConsole = false;
}

ConsoleApp::~ConsoleApp() {
	if (screenBuffer != nullptr) {
		delete[] screenBuffer;
		screenBuffer = nullptr;
	}
}

void ConsoleApp::ResetConsole() {
	// Reset Window Size to Minimum
	windowSize = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(handleOut, TRUE, &windowSize);

	// Set Default Screen Buffer Size
	screenSize = { 120, 9001 };
	SetConsoleScreenBufferSize(handleOut, screenSize);
	/*SetConsoleActiveScreenBuffer(handleOut);*/

	// Set Default Font Settings
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize = COORD({ 8, 16 });
	fontInfo.FontFamily = 0x00000036;
	fontInfo.FontWeight = FW_NORMAL;
	wcscpy_s(fontInfo.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(handleOut, FALSE, &fontInfo);

	// Set Default Window Size
	windowSize = { 0, 0, 119, 29 };
	SetConsoleWindowInfo(handleOut, TRUE, &windowSize);

	// Set Default Console Mode
	SetConsoleMode(handleIn, ENABLE_EXTENDED_FLAGS | ENABLE_QUICK_EDIT_MODE | ENABLE_INSERT_MODE);

	// Set Default Cursor Mode
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 25;
	cursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(handleOut, &cursorInfo);

	// Disable Maximizing & Resizing Window
	HWND consoleWindow = GetConsoleWindow();
	LONG windowStyle = GetWindowLong(consoleWindow, GWL_STYLE);
	SetWindowLong(consoleWindow, GWL_STYLE, windowStyle | WS_MAXIMIZEBOX | WS_SIZEBOX);
}

void ConsoleApp::Error(const wchar_t* message) {
	ResetConsole();
	SetConsoleTitle(L"Errors!!!");
	invalidConsole = true;
	wprintf(L"%ls\n\n", message);
	wprintf(L"Press ENTER to exit...");
	while (!GetAsyncKeyState(VK_RETURN));
}

void ConsoleApp::Start() {
	if (!running && !invalidConsole) {
		running = true;
		updateBuffer = true;
		Run();
	}
}

void ConsoleApp::Close() {
	running = false;
}

void ConsoleApp::Run() {
	// Frame Duration
	float deltaTime = 1;

	// OnCreate
	OnCreate();

	// Start App Loop
	while (running) {
		// Start Frame Timer
		auto startFrameTime = std::chrono::high_resolution_clock::now();

		// Get Input
		GetInput();

		// OnUpdate
		OnUpdate();

		// Draw to Console Screen Buffer
		if (updateBuffer) {
			WriteConsoleOutput(handleOut, screenBuffer, screenSize, { 0, 0 }, &windowSize);
			updateBuffer = false;
		}

		// Update Title
		std::wstring title = appName + L" - FPS: " + std::to_wstring(int(round(1 / deltaTime)));
		SetConsoleTitle(title.c_str());

		// Stop Frame Timer
		auto endFrameTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> frameTime = endFrameTime - startFrameTime;
		deltaTime = frameTime.count();
	}
}

void ConsoleApp::GetInput() {
	// Create Input Buffer
	INPUT_RECORD* inputBuffer = nullptr;
	DWORD numEvents = 0;

	// Get Number of Unread Console Input Events
	GetNumberOfConsoleInputEvents(handleIn, &numEvents);

	// Check for Existing Events
	if (numEvents != 0) {
		// Read Console Input
		inputBuffer = new INPUT_RECORD[numEvents];
		ReadConsoleInput(handleIn, inputBuffer, numEvents, &numEvents);
	}

	// Get Mouse & Keyboard Inputs (open a thread for each)
	std::thread mouseInputThread(&ConsoleApp::GetMouseInput, this, inputBuffer, numEvents);
	std::thread keyboardInputThread(&ConsoleApp::GetKeyboardInput, this);

	// Wait for Threads
	mouseInputThread.join();
	keyboardInputThread.join();

	// Free Input Buffer
	if (inputBuffer != nullptr)
		delete[] inputBuffer;
}

void ConsoleApp::GetMouseInput(INPUT_RECORD* inputBuffer, DWORD numEvents) {
	// Update Buttons Old State
	for (short i = 0; i < mouseInput.numButtons; ++i)
		mouseInput.buttons[i].oldState = mouseInput.buttons[i].newState;

	// Check for Mouse Events
	for (DWORD i = 0; i < numEvents; ++i) {
		switch (inputBuffer[i].EventType) {
		case MOUSE_EVENT:
			// Update Mouse Position
			if (inputBuffer[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
				mouseInput.position = inputBuffer[i].Event.MouseEvent.dwMousePosition;
			// Update Buttons State
			mouseInput.buttons[0].newState = (inputBuffer[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED);
			mouseInput.buttons[1].newState = (inputBuffer[i].Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED);
			break;
		}
	}
}

void ConsoleApp::GetKeyboardInput() {
	// Loop Through Keys
	for (short i = 0; i < keyboardInput.numKeys; ++i) {
		// Update Old Key State
		keyboardInput.keys[i].oldState = keyboardInput.keys[i].newState;
		// Update New Key State
		keyboardInput.keys[i].newState = GetAsyncKeyState(i) & 0x8000;
	}
}


Vec2 ConsoleApp::GetMousePosition() const {
	return Vec2((int)mouseInput.position.X, (int)mouseInput.position.Y);
}
bool ConsoleApp::KeyPressed(const unsigned char keyCode) const {
	if (keyCode == VK_LBUTTON || keyCode == VK_RBUTTON)
		return mouseInput.buttons[keyCode - 1].newState;
	return keyboardInput.keys[keyCode].newState;
}
bool ConsoleApp::KeyReleased(const unsigned char keyCode) const {
	if (keyCode == VK_LBUTTON || keyCode == VK_RBUTTON)
		return mouseInput.buttons[keyCode - 1].oldState && !mouseInput.buttons[keyCode - 1].newState;
	return keyboardInput.keys[keyCode].oldState && !keyboardInput.keys[keyCode].newState;
}
bool ConsoleApp::KeyHeld(const unsigned char keyCode) const {
	if (keyCode == VK_LBUTTON || keyCode == VK_RBUTTON)
		return mouseInput.buttons[keyCode - 1].oldState && mouseInput.buttons[keyCode - 1].newState;
	return keyboardInput.keys[keyCode].oldState && keyboardInput.keys[keyCode].newState;
}


// TOOLS
int ConsoleApp::GetScreenWidth() const { return (int)screenSize.X; }
int ConsoleApp::GetScreenHeight() const { return (int)screenSize.Y; }

bool ConsoleApp::OutOfBounds(int x, int y) const { return (x < 0 || y < 0) || (x >= screenSize.X || y >= screenSize.Y); }

void ConsoleApp::SetPixel(int x, int y, wchar_t glyph, unsigned char color) {
	if (OutOfBounds(x, y))
		return;

	screenBuffer[x + screenSize.X * y].Char.UnicodeChar = glyph;
	screenBuffer[x + screenSize.X * y].Attributes = color;

	updateBuffer = true;
}
void ConsoleApp::SetPixel(Vec2 vec2, wchar_t glyph, unsigned char color) {
	if (OutOfBounds(vec2.x, vec2.y))
		return;

	screenBuffer[vec2.x + screenSize.X * vec2.y].Char.UnicodeChar = glyph;
	screenBuffer[vec2.x + screenSize.X * vec2.y].Attributes = color;

	updateBuffer = true;
}
void ConsoleApp::Fill(int x0, int y0, int x1, int y1, wchar_t glyph, unsigned char color) {
	for (short x = (short)x0; x <= (short)x1; ++x)
		for (short y = (short)y0; y <= (short)y1; ++y)
			if (!OutOfBounds(x, y))
				SetPixel(x, y, glyph, color);
}
void ConsoleApp::Fill(Vec2 vec2_0, Vec2 vec2_1, wchar_t glyph, unsigned char color) {
	for (short x = (short)vec2_0.x; x <= (short)vec2_1.x; ++x)
		for (short y = (short)vec2_0.y; y <= (short)vec2_1.y; ++y)
			if (!OutOfBounds(x, y))
				SetPixel(x, y, glyph, color);
}

void ConsoleApp::DrawBuffer(CHAR_INFO* buffer, COORD bufferSize, SMALL_RECT region) {
	for (short x = 0; x < bufferSize.X && x + region.Left <= region.Right; ++x)
		for (short y = 0; y <= bufferSize.Y && y + region.Top <= region.Bottom; ++y)
			if (!OutOfBounds(x, y))
				SetPixel(x + region.Left, y + region.Top, buffer[x + screenSize.X * y].Char.UnicodeChar, (unsigned char)buffer[x + screenSize.X * y].Attributes);
}

void ConsoleApp::ClearScreen() {
	for (short x = 0; x < screenSize.X; ++x)
		for (short y = 0; y < screenSize.Y; ++y)
			SetPixel(x, y, NULL, 0x0000);
}

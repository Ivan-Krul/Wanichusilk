#include "Window.h"
#include <stdexcept>

bool Window::create(const char* name, int width, int height, SDL_WindowFlags flags) noexcept {
#ifdef PRECHECK_INSTANCE
	if ((SDLRoot::getInstance() & (Uint32)SDLRoot::InitPart::video) == 0) return false;
#endif

#pragma warning(suppress : 4554)
	if (width >> sizeof(width) * 8 - 1) width = ((width << 1) >> 1);
#pragma warning(suppress : 4554)
	if (height >> sizeof(height) * 8 - 1) height = ((height << 1) >> 1);

	auto name_len = strlen(name);
	if (mWindowName == nullptr)
		mWindowName = new char[name_len + 1];
	else if (strlen(mWindowName) != name_len) {
		delete[] mWindowName;
		mWindowName = new char[name_len + 1];
	}

	memcpy(mWindowName, name, name_len);
	mWindowName[name_len] = 0;

	//Create window
	if ((mpWindow = SDL_CreateWindow(mWindowName, width, height, flags)) == nullptr)
		return false;
	else
		if (!createRenderer()) return false; // Get window surface

	if (mfOnCreate)
		mfOnCreate();


	return true;
}

void Window::terminate() {
	if (mpWindow == nullptr) return;

	SDL_DestroyWindow(mpWindow);

	if (mWindowName)
		delete[] mWindowName;

	terminateRenderer();
	mpWindow = nullptr;
	mWindowName = nullptr;

	if (mfOnDestroy)
		mfOnDestroy();
}

bool Window::createRenderer() {
	return (mpRenderer = SDL_GetRenderer(mpWindow)) == nullptr;
}

void Window::terminateRenderer() {
	SDL_DestroyRenderer(mpRenderer);
	mpRenderer = nullptr;
}

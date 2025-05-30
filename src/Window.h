#pragma once
#include <SDL3/SDL.h>

// thought, GPU side or CPU
// doesn't matter, so GPU

class Window {
public:
	inline Window() noexcept = default;

	bool create(const char* name, int width, int height, SDL_WindowFlags flags = NULL) noexcept;

	inline int getWidth() const noexcept { return mWindowWidth; }
	inline int getHeight() const noexcept { return mWindowHeight; }

	void setOnCreate(void(*onCreate)(void)) { mfOnCreate = onCreate; }
	void setOnDestroy(void(*onDestroy)(void)) { mfOnDestroy = onDestroy; }

	SDL_Window* getWindow() { return mpWindow; }
	SDL_Renderer* getWindowRenderer() { return mpRenderer; }

	void terminate();

	~Window() {
		terminate();
	}
protected:
	bool createRenderer();
	void terminateRenderer();

protected:
	SDL_Window* mpWindow = nullptr;

	int mWindowWidth = -1;
	int mWindowHeight = -1;

	char* mWindowName = nullptr;

	void(*mfOnCreate)(void) = nullptr;
	void(*mfOnDestroy)(void) = nullptr;

	SDL_Renderer* mpRenderer = nullptr;
};


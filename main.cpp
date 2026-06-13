#include "raylib.h"
#include "chip8.h"




int main() {
	Chip8 cpu;
	cpu.initialize();

	int w = 800;
	int h = 450;

	InitWindow(w, h, "chip8");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
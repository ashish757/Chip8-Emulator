#include "raylib.h"
#include "chip8.h"




int main() {
	Chip8 cpu;
	cpu.initialize();

	int scaleFactor = 15;
	InitWindow(64*scaleFactor, 32 * scaleFactor, "Chip-8 Emulator");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {

		// key binding from chip8 to host
		cpu.keypad[0x1] = IsKeyDown(KEY_ONE) ? 1 : 0;
		cpu.keypad[0x2] = IsKeyDown(KEY_TWO) ? 1 : 0;
		cpu.keypad[0x3] = IsKeyDown(KEY_THREE) ? 1 : 0;
		cpu.keypad[0xC] = IsKeyDown(KEY_FOUR) ? 1 : 0;

		cpu.keypad[0x4] = IsKeyDown(KEY_Q) ? 1 : 0;
		cpu.keypad[0x5] = IsKeyDown(KEY_W) ? 1 : 0;
		cpu.keypad[0x6] = IsKeyDown(KEY_E) ? 1 : 0;
		cpu.keypad[0xD] = IsKeyDown(KEY_R) ? 1 : 0;

		cpu.keypad[0x7] = IsKeyDown(KEY_A) ? 1 : 0;
		cpu.keypad[0x8] = IsKeyDown(KEY_S) ? 1 : 0;
		cpu.keypad[0x9] = IsKeyDown(KEY_D) ? 1 : 0;
		cpu.keypad[0xE] = IsKeyDown(KEY_F) ? 1 : 0;

		cpu.keypad[0xA] = IsKeyDown(KEY_Z) ? 1 : 0;
		cpu.keypad[0x0] = IsKeyDown(KEY_X) ? 1 : 0;
		cpu.keypad[0xB] = IsKeyDown(KEY_C) ? 1 : 0;
		cpu.keypad[0xF] = IsKeyDown(KEY_V) ? 1 : 0;

// basically we trying to speed up the execution speed
		for (int i = 0; i<10; i++) {
			cpu.executeCycle();
		}

		if (cpu.delayTimer > 0) cpu.delayTimer--;
		if (cpu.soundTimer > 0) cpu.soundTimer--;


		BeginDrawing();
		ClearBackground(BLACK);


		for (int y =0; y < 32; y++) {
			for (int x =0; x<64; x++) {
				if (cpu.displayBuffer[(y*64) + x] == 1) {
					DrawRectangle(x * scaleFactor, y * scaleFactor, scaleFactor , scaleFactor, WHITE);
				}
			}
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
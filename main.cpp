#include <iostream>
#include "raylib.h"
#include "chip8.h"
#include <string>
#include <vector>


enum AppState {MENU, RUNNING};


int main() {
	Chip8 cpu;
	int scaleFactor = 15;
	InitWindow(64*scaleFactor, 32 * scaleFactor + 10, "Chip-8 Emulator");
	SetTargetFPS(60);

	AppState currentState = MENU;

	std::vector< std::pair<std::string, std::string> >  romList = {
		{"../Keypad Test [Hap, 2006].ch8", "Keypad Test"},
		{"../Pong one Player.ch8", "Pong"},
	};

	int selectedROMIndex = 0;
	int cyclesPerFrame = 10;


	while (!WindowShouldClose()) {
		if (currentState == MENU) {
			if (IsKeyPressed(KEY_DOWN)) {
				selectedROMIndex = (selectedROMIndex + 1) % romList.size();
			}
			if (IsKeyPressed(KEY_UP)) {
				selectedROMIndex = (selectedROMIndex - 1 + romList.size()) % romList.size();
			}

			if (IsKeyPressed(KEY_RIGHT)) cyclesPerFrame++;
			if (IsKeyPressed(KEY_LEFT) && cyclesPerFrame > 5) cyclesPerFrame--;

			if (IsKeyPressed(KEY_ENTER)) {
				cpu.initialize();
				if (cpu.loadRom(romList[selectedROMIndex].first)) {
					currentState = RUNNING;
				}
			}

			BeginDrawing();
			ClearBackground(RAYWHITE);

			DrawText("CHIP-8 LAUNCHER", 40, 40, 40, BLACK);

			for (int i =0; i < romList.size(); i++) {
				Color textColor = (i == selectedROMIndex) ? BLACK : GRAY;

				if (i == selectedROMIndex) {
					DrawText(">", 20, 120 + (i * 30), 30, GREEN);
				}

				DrawText(romList[i].second.c_str(), 40, 120 + (i * 30), 20, textColor);
			}

			std::string speedText = "CPU Speed:  (Left/Right to adjust)";
			std::string infoText = std::to_string(cyclesPerFrame) + " cycles per frame";
			DrawText(speedText.c_str(), 40, 300, 20, BLACK);
			DrawText(infoText.c_str(), 40, 330, 20, ORANGE);

			DrawText("Pres ENTER to Launch", 40, 420, 20, GREEN);

			EndDrawing();

		} else if (currentState == RUNNING) {
			// key binding from chip8 to host
			if (IsKeyPressed(KEY_BACKSPACE)) {
				currentState = MENU;
			}

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
			for (int i = 0; i<cyclesPerFrame; i++) {
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

			DrawText("Press BACKSPACE for Menu", 800, 470, 10, RAYWHITE);

			EndDrawing();
		}

		}


	CloseWindow();

	return 0;
}
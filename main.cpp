#include <iostream>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "chip8.h"
#include <string>
#include <vector>

std::string getRomPath();

enum AppState {MENU, RUNNING};

int main() {
    Chip8 cpu;
    int scaleF = 15;
    InitWindow(64 * scaleF, 32 * scaleF + 10, "Chip-8 Emulator");
    SetTargetFPS(60);

    const char* appDir = GetApplicationDirectory();
    std::string fontPath = std::string(appDir) + "font.ttf";
    Font cFont = LoadFontEx(fontPath.c_str(), 32, nullptr, 0);

    GuiSetFont(cFont);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    AppState appState = MENU;

    std::string binDir = std::string(appDir) + "bin/";

    std::vector<std::pair<std::string, std::string>> roms = {
        {binDir + "Keypad Test [Hap, 2006].ch8", "Keypad Test"},
        {binDir + "Tic-Tac-Toe [David Winter].ch8", "Tic-Tac-Toe"},
        {binDir + "Space Invaders.ch8", "Space Invaders"},
        {binDir + "Tetris.ch8", "Tetris"},
        {binDir + "Pong one Player.ch8", "Pong"},
        {binDir + "Cave.ch8", "Cave"},
        {binDir + "PBrix.ch8", "Brix"},
        {binDir + "Worm V4.ch8", "Worm"}
    };

    std::string listStr = "";
    for (size_t i = 0; i < roms.size(); i++) {
        listStr += roms[i].second;
        if (i != roms.size() - 1) listStr += ";";
    }

    int selRomIdx = 0;
    int scrollIdx = 0;
    int cycPerFrame = 10;
    bool spinEdit = false;
    bool hasErr = false;

    while (!WindowShouldClose()) {
        if (appState == MENU) {
            BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            DrawTextEx(cFont, "CHIP-8 LAUNCHER", {40, 40}, 40, 2, BLACK);
            DrawTextEx(cFont, "Select a program or import a custom file", {40, 90}, 20, 1, DARKGRAY);

            DrawTextEx(cFont, "CPU Speed ", {40, 145}, 22, 1, DARKGRAY);
            if (GuiSpinner(Rectangle{ 160, 140, 140, 35 }, "", &cycPerFrame, 5, 100, spinEdit)) {
                spinEdit = !spinEdit;
            }



            if (GuiButton(Rectangle{ 40, 200, 260, 50 }, "IMPORT ROM")) {
                std::string path = getRomPath();
                if (!path.empty()) {
                    size_t idx = path.find_last_of("/\\");
                    std::string name = (idx != std::string::npos) ? path.substr(idx + 1) : path;
                    roms.push_back({path, name});

                    listStr = "";
                    for (size_t i = 0; i < roms.size(); i++) {
                        listStr += roms[i].second;
                        if (i != roms.size() - 1) listStr += ";";
                    }
                    selRomIdx = roms.size() - 1;
                    hasErr = false;
                }
            }

            if (GuiButton(Rectangle{ 40, 270, 260, 50 }, "LAUNCH")) {
                cpu.initialize();
                if (cpu.loadRom(roms[selRomIdx].first)) {
                    appState = RUNNING;
                    hasErr = false;
                } else {
                    hasErr = true;
                }
            }

            if (hasErr) {
                DrawTextEx(cFont, "ERROR: Load Failed", {40, 350}, 18, 1, RED);
            }

            DrawTextEx(cFont, "Program List", {500, 40}, 30, 2, BLACK);
            GuiListView(Rectangle{ 500, 80, 420, 360 }, listStr.c_str(), &scrollIdx, &selRomIdx);

            EndDrawing();

        } else if (appState == RUNNING) {
            if (IsKeyPressed(KEY_BACKSPACE)) {
                appState = MENU;
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

            for (int i = 0; i < cycPerFrame; i++) {
                cpu.executeCycle();
            }

            if (cpu.delayTimer > 0) cpu.delayTimer--;
            if (cpu.soundTimer > 0) cpu.soundTimer--;

            BeginDrawing();
            ClearBackground(BLACK);

            for (int y = 0; y < 32; y++) {
                for (int x = 0; x < 64; x++) {
                    if (cpu.displayBuffer[(y * 64) + x] == 1) {
                        DrawRectangle(x * scaleF, y * scaleF, scaleF, scaleF, WHITE);
                    }
                }
            }

            if (GuiButton(Rectangle{ 10, 10, 80, 30 }, "BACK")) {
                appState = MENU;
            }

            EndDrawing();
        }
    }

    UnloadFont(cFont);
    CloseWindow();
    return 0;
}



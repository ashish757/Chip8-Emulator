#pragma once
#include <string>
#include <cstdint>

struct Chip8 {
	uint8_t memory[4096];
	uint8_t registers[16];
	uint16_t indexRegister;
	uint16_t stackPointer;
	uint16_t programCounter;
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint16_t stack[16];
	uint8_t displayBuffer[64 * 32];
	uint8_t keypad[16];

	void initialize();
	bool loadRom(const std::string &filename);
	void executeCycle();
};
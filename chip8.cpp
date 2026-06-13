#include "chip8.h"
#include <algorithm>
#include <fstream>

void Chip8::initialize() {
	std::fill(std::begin(memory), std::end(memory), 0);
	std::fill(std::begin(registers), std::end(registers), 0);
	std::fill(std::begin(stack), std::end(stack), 0);
	std::fill(std::begin(displayBuffer), std::end(displayBuffer), 0);

	indexRegister = 0;
	programCounter = 0;
	delayTimer = 0;
	soundTimer = 0;
	stackPointer = 0;
}

bool Chip8::loadRom(const std::string &filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		return false;
	}

	std::streampos fileSize = file.tellg();
	long maxAllowedSize = 4096 - 0x200;

	if (fileSize > maxAllowedSize) {
		file.close();
		return false;
	}

	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(&memory[0]), fileSize);
	file.close();
	return true;
}


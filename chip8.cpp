#include "chip8.h"
#include <algorithm>
#include <fstream>
#include <iostream>

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


void Chip8::executeCycle() {
	uint16_t currentOpcode = (memory[programCounter] << 8) | memory[programCounter+1];
	programCounter += 2;

	uint16_t firstNibble = (currentOpcode & 0xF000) >> 12;
	uint16_t registerX = (currentOpcode & 0xF000) >> 8;
	uint16_t registerY = (currentOpcode & 0xF000) >> 4;

	uint16_t constantValueKK = currentOpcode & 0x0FFF;
	uint16_t addressNNN = currentOpcode & 0x0FFF;

	switch (firstNibble) {
		case 0x0:
			if (constantValueKK == 0xE00) {
				std::fill(std::begin(displayBuffer), std::end(displayBuffer), 0);
			} else if (constantValueKK == 0xE00) {
				stackPointer--;
				programCounter = stack[stackPointer];
			}
			break;
		case 0x1:
			programCounter = addressNNN;
			break;
		case 0x2:
			stack[stackPointer] = programCounter;
			stackPointer++;
			programCounter +=2;
		case 0x3:
			if (registers[registerX] == constantValueKK ) {
				programCounter+=2;
			}
		case 0x4:
			if (registers[registerX] != constantValueKK ) {
				programCounter+=2;
			}
		case 0x5:
			if (registers[registerX] != registers[registerY]) {
				programCounter+=2;
			}
		case 0x6:
			registers[registerX] = constantValueKK;
			break;
		case 0x7:
			registers[registerX] += constantValueKK;
			break;
		case 0xA:
			indexRegister = addressNNN;
			break;
		case 0xD:

		default:
			std::cout<<"Unknown opcode! "<< std::hex<<currentOpcode<<std::endl;
			break;
	}



}

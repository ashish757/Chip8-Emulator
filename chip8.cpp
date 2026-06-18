#include "chip8.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include  <cstdlib>

// Chip 8 fontset
const uint8_t fontset[80] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::initialize() {
	std::fill(std::begin(memory), std::end(memory), 0);
	std::fill(std::begin(registers), std::end(registers), 0);
	std::fill(std::begin(stack), std::end(stack), 0);
	std::fill(std::begin(displayBuffer), std::end(displayBuffer), 0);
	std::fill(std::begin(keypad), std::end(keypad), 0);


	for (int i = 0; i < 80; i++) {
		memory[i] = fontset[i];
	}

	indexRegister = 0;
	programCounter = 0x200;
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

	std::vector<char> buffer(fileSize);

	if (file.read(buffer.data(), fileSize)) {
		for (int i = 0; i < fileSize; i++) {
			memory[0x200+i] = static_cast<uint8_t>(buffer[i]);
		}

		std::cout << "ROM Loaded "<<filename << std::endl;
		return true;

	}

	std::cerr<<"Failed to load rom file "<<filename<<std::endl;
	return false;
}


void Chip8::executeCycle() {
	// we grab the first two char (8 bits), and then left shit by 8 bit(2 places), and then attack bY OR the other two chars
	uint16_t currentOpcode = (memory[programCounter] << 8) | memory[programCounter+1];
	programCounter += 2;

	uint16_t firstNibble = (currentOpcode & 0xF000) >> 12;
	uint16_t registerX = (currentOpcode & 0x0F00) >> 8;
	uint16_t registerY = (currentOpcode & 0x00F0) >> 4;

	uint16_t constantValueKK = currentOpcode & 0x00FF;
	uint16_t addressNNN = currentOpcode & 0x0FFF;
	uint16_t variant = currentOpcode & 0x000F;

	switch (firstNibble) {
		case 0x0:
			if (constantValueKK == 0xE0) {
				std::fill(std::begin(displayBuffer), std::end(displayBuffer), 0);
			} else if (constantValueKK == 0xEE) {
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
			programCounter = addressNNN;
			break;
		case 0x3:
			if (registers[registerX] == constantValueKK ) {
				programCounter+=2;
			}
			break;
		case 0x4:
			if (registers[registerX] != constantValueKK ) {
				programCounter+=2;
			}
			break;
		case 0x5:
			if (registers[registerX] == registers[registerY]) {
				programCounter+=2;
			}
			break;
		case 0x6:
			registers[registerX] = constantValueKK;
			break;
		case 0x7:
			registers[registerX] += constantValueKK;
			break;
		case 0x8: {
			uint16_t variant = currentOpcode & 0x000F;
			switch (variant) {
				case 0x0:
					registers[registerX] = registers[registerY];
					break;
				case 0x1:
					registers[registerX] |= registers[registerY];
					break;
				case 0x2:
					registers[registerX] &= registers[registerY];
					break;
				case 0x3:
					registers[registerX] ^= registers[registerY];
					break;
				case 0x4: {
					uint16_t sum = registers[registerX] + registers[registerY];
					registers[0xF] = (sum > 255) ? 1 : 0;
					registers[registerX] = sum & 0xFF;
					break;
				}
				case 0x5:
					registers[0xF] = (registers[registerX] >= registers[registerY]) ? 1 : 0;
					registers[registerX] -= registers[registerY];
					break;
				case 0x6:
					registers[0xF] = registers[registerX] & 0x1;
					registers[registerX] >>= 1;
					break;
				case 0x7:
					registers[0xF] = (registers[registerY] >= registers[registerX]) ? 1 : 0;
					registers[registerX] = registers[registerY] - registers[registerX];
					break;
				case 0xE:
					registers[0xF] = (registers[registerX] & 0x80) >> 7;
					registers[registerX] <<= 1;
					break;
			}
			break;
		}
		case 0xA:
			indexRegister = addressNNN;
			break;
		case 0xE:
			if (constantValueKK == 0x9E) {
				if (keypad[registers[registerX]] != 0) {
					programCounter += 2;
				}
			} else if (constantValueKK == 0xA1) {
				if (keypad[registers[registerX]] == 0) {
					programCounter += 2;
				}
			}
			break;
		case 0xF: {
			switch (constantValueKK) {
				case 0x07:
					registers[registerX] = delayTimer;
					break;
				case 0x0A: {
					bool keypress = false;
					for (int i =0; i<16; i++) {
						if (keypad[i] != 0) {
							registers[registerX] = i;
							keypress = true;
							break;
						}
					}
					if (!keypress) {
						programCounter -= 2;
					}
					break;
				}

				case 0x15:
					delayTimer = registers[registerX];
					break;
				case 0x18:
					soundTimer = registers[registerX];
					break;
				case 0x1E:
					indexRegister += registers[registerX];
					break;
				case 0x29:
					indexRegister = registers[registerX] * 5;
					break;
				case 0x33:
					memory[indexRegister] = registers[registerX] / 100;
					memory[indexRegister + 1] = (registers[registerX] / 10) % 10;
					memory[indexRegister + 2] = registers[registerX] % 10;
					break;
				case 0x55:
					for (int i = 0; i<=registerX; i++) {
						memory[indexRegister + i] = registers[i];
					}
					break;
				case 0x65:
					for (int i = 0; i<=registerX; i++) {
						registers[i] = memory[indexRegister + i];
					}
					break;
			}
			break;
		}
		case 0xB:
			programCounter = addressNNN + registers[0];
			break;

		case 0xC: {
			uint8_t randomByte = rand() % 256;
			registers[registerX] = randomByte & constantValueKK;
			break;

		}
		case 0xD: {
			uint16_t xPos = registers[registerX] % 64;
			uint16_t yPos = registers[registerY] % 32;

			uint16_t spriteHeight = variant;
			registers[0xF] = 0;
			for (unsigned int row = 0; row < spriteHeight; row++) {
				uint8_t spriteByte = memory[indexRegister + row];

				for (unsigned int col = 0; col<8; col++) {
					uint8_t spritePixel = spriteByte & (0x80 >> col);

					int displayIndex = ((yPos+row) * 64) + (xPos + col);

					if (xPos + col >=64 || yPos + row >= 32) {
						continue;
					}

					if (spritePixel) {
						if (displayBuffer[displayIndex] == 1) {
							registers[0xF] = 1;
						}
						displayBuffer[displayIndex] ^= 1;
					}
				}
			}
			break;
		}

		default:
			std::cout<<"Unknown opcode! "<< std::hex<<currentOpcode<<std::endl;
			break;
	}



}

# Technical Architecture: CHIP-8 Emulator in C++

## 1. High-Level Concept
A CHIP-8 emulator acts as an intermediary layer that translates instructions meant for an ancient 1970s 8-bit architecture into operations modern computers can execute. Instead of just being an interpreted language, CHIP-8 mimics an entire CPU architecture, complete with its own memory constraints and registers.

If we have a software (`S`) meant for the CHIP-8 system (`C1`), it will naturally fail on a modern system (`C2`). The emulator intercepts the `C1` machine code, translates the instructions, and allows `C2` to process them seamlessly.

## 2. Memory & The Execution Cycle
Because CHIP-8 is extremely lightweight, the system only requires 4096 bytes (4KB) of RAM.
* **Memory Structure:** The RAM is simulated using a modern C++ data structure (an array of size 4096). The first 200 spaces are reserved for internal CHIP-8 system use, leaving the rest open for loading the `.ch8` binary files.
* **The Execution Loop:** The core of the emulator is a continuous `while` loop that drives the CPU clock. Inside the `executeCycle` function, a `programCounter` variable tracks the current execution position in memory and calculates the next instruction to fetch.
```
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM
```

# Register
- A register is like a storage place very near to the CPU, in chip 8 there are 16 such registers, 8 bit each.
- CPU can only perform operations on the data which is in the register (not RAM).
- in chip8 registers can store only max value of 255 (11111111).
- To overcome this limit, 16th register is used a flag, to indicate this overflow

## 3. Instruction Decoding & Bitwise Magic
CHIP-8 has exactly 35 standard operational codes (opcodes). Because each instruction is exactly two bytes long, the emulator parses the raw machine code and chops it into 2-byte groups.

To extract the specific commands from these hexadecimals (where every character is a 4-bit nibble), we rely heavily on bitwise operations:
* **Bitwise AND (`&`):** Used as a mask to isolate specific values. For example, applying a bitwise AND with the mask `0xFFFF` on `0xA22A` strips away unwanted bits, keeping only the values aligned with the `F`s.
* **Bitwise Right Shift (`>>`):** Used to slide characters over and eliminate trailing zeroes. Because a hex character is 4 bits, shifting `0xA000 >> 12` yields `0xA`.
* **Routing:** Once the defining nibble is extracted, it is fed into a massive `switch` case that maps the ancient CHIP-8 command to its modern C++ equivalent (e.g., executing a standard addition routine).

## 4. Graphics Engine & Collision Detection
* **Display Constraints:** The display is a simple 64x32 pixel black-and-white grid. Since this is incredibly small on modern monitors, a visual multiplier is used to scale it up. Pixels are strictly binary: `1` for white and `0` for black.
* **Immediate Mode Rendering:** Unlike the DOM in web development, there are no persistent UI elements. The game loop aggressively erases and redraws the entire screen 60 times per second, which creates the classic "flashing" effect of retro games.
* **Collision via Bitwise ZOR (`^`):** When drawing sprites, the emulator compares the *current* pixel value with the *new* pixel value using the `^` (XOR) operator. If two white pixels overlap (`1 ^ 1 = 0`), it registers a collision, and the 16th CPU register (`Vf`) is set to `1`.

## 5. User Interface & Hardware Mapping
* **The Keypad:** The original 16-key hex pad (1-C, 4-D, 7-E, A-F) is mapped 1:1 onto the modern QWERTY left hand (1-4, Q-R, A-F, Z-V).
* **UI Controls:** To avoid the nightmare of manually tracking mouse coordinates 60 times a second to detect button hovers, the emulator utilizes `raygui` to abstract the heavy UI math.
* **Native Dialogs:** To load custom `.ch8` files, the emulator hooks into native OS file pickers—utilizing `AppleScript` via macOS and spawning a background `shell` on Windows.





### I used AI to improve writing and grammar, you can find below, the original `technical detail` which i wrote

https://docs.google.com/document/d/1JY9W7jFz--8trjnsDtCjEP6NaZTUCz2OtxtFdhQBUJY/edit?usp=sharing
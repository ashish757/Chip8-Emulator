# Technical Architecture: CHIP-8 Emulator in C++

# What is chip 8?
> chip 8 is basically a interpreted programing language of ancient times (70s). It kind of mimics a small computer (8 bit), limiting its max memory to 4KB. But the special thing is, its not just a language, it mimics a CPU architecture, making it like a different computer system which doesnt exists phycally, but uses other physical computer to run its own acrhitecture.

# What is Emulator?
> Well think of emulator as a extra added layer between the software and low level computer systems. which does some magic to run a software which is not ment to run on that system.

> Windows Application –> macOS (Won’t Run)

> Windows Application –> Emulator –> macOS (Will Run)

## Execution Flow
- Lets say we have a software S, designed to run on System of type C1, but now we want to run the same software on a different system lets say C2,
- S –> C1 RUNs properly
- S –> C2 Fails as C2 cant understand the instruction that software S is trying to give to C2

## Emulator in the picture
- Software S (instruction codes for C1 ) –> Emulator (Translates Instruction code for C1 to C2) –> now C2 can run (Translated instructions).


# Now Chip 8 and Emulator
> Lets say you have a program which was designed in and for Chip8, but now you dont have a chip8 system, how do you run this program?

> We basically use an emulator to trick the software to thing that its running in the old Chip8 system but its acctually running on moder computers.

> Howwwwww? well Chip8 has 35 standard instructions and only 4096 bytes of memory, so its faily easy to map each and every of its instruction and instead do it in the modern langauges (like i have used C++).

> We store the data in modern C++ Data Structures, but we trick the target software to thing that it has dorect acces to the RAM of the computer.

# How emulator achieves this?
### Memory
- we store the data in modern Data structures and tell the software that its data is stored directly in the ram.
- create and array of size 4096 (4Kb), we leave starting 200, and start from 201rth index, the  200 spaces are used by the Chip8 itself, rest is left to be used by the program.

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


### Registers
- There are 16 registers each of size 8 bit, named as V0 V1 ... V15
- V15 is reserved for flag, such as integer overflow and collision depending upon context.

### Processing
- Chip 8 has only 37 different commands, which we will manually define and translate the working in C++ language.
- For example code 8XYN represents addition, so we will intercept this instruction code and do the job(addition), according to the moder computer and return the ans, tricking software into thinking that it got its answer processes from the CPU.

### Flow
> INPUT: Machine code ment for Chip8 -> MEMORY: store in modern data structures -> PROCESS: intercept the instruction codes and then process and return result -> OUTPUT: use a screen to display basic graphics with the calculated results

# Retrival of instructions

## A brief on Hexadecimal
- 0x is used to represent a Hexadecimal number, each hex char is 4 bits.
- A nibble(4 bits) gets translated into one hexadecimal number.

## MORE on Memory & processing
- we receive machine code, we parse it, chop up the all the instruction, into two byte groups,
- These two bute groups, indicate one instruction.
- We create a big switch case, where we map these instruction with our memory.

## Retrival of specific parts of instruction  (Bitwise &)
- Bitwise AND only keeps the values when mask has F in it
- Lets say 0xA22A is an instruction to get the last four letter we take a bitwise AND with mask 0XFFFF

## Sliding the chars (Bitwise >>)
- Now that we have the required character/Nibble out of a instruction code, we need to eliminate the extra 0 values.
- We do this with right shift(>>), we simply do >> 4 (4, as a hex is of 4 bits) for removing a single zero on the right.
- 0xA000 >> 12 = 0xA

> so the bottom line is, we take a Nibble out of the instruction code, this single char tell us about the kind of operation we need to perform for this instruction code. We look for it in our switch statement, if ww have it then we execute it.

# Graphics
- we basically have black and white screen, 64px length and 32px height (we use multiplier so the size is not exactly 64x32, as that will be two small)
- we then have 1s, 0s to represent black and white pixels
- the engine basically deleted every pixel and re draws it again, this causes a flashing effect as moder screens are faster and also show the transition.

# Collision detection
- the value of a pixel is set to 1 (white) and 0 (black)
- We take XOR of the Current value of the pixel and the to be updated value
- if two pixels are white (1) the xor will be 0, and when this happens we set the Vf to 1, indicating a collision has occured.
- Vf is the 16th register which is left empty and use only to show collision.


# Keypad
```angular2html
Original Chip8       ---->      Modern keyboard
1  2   3   C                    1   2   3   4
4  5   6   D                    Q   W   E   R
7  8   9   E                    A   S   D   F
A  0   B   F                    Z   X   C   V
```


### I used AI to improve writing and grammar, you can find below, the original `technical detail` which i wrote myself

https://docs.google.com/document/d/1JY9W7jFz--8trjnsDtCjEP6NaZTUCz2OtxtFdhQBUJY/edit?usp=sharing
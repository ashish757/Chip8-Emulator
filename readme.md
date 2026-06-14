# What is a chip 8 emulator?
- Emulator are basically software layers to trick other software into thinking that they are being run on another computer, the one that they support natively
- Here Chip 8 emulator will trick some codes/softawres which were originally created for Chip 8, to think that they are running on the chip 8, but in reality they will be running on moder computers.

# How emulator achieves this?
### Memory 
- we store the data in modern Data structures and tell the software that its data is stored directly in the ram.
- create and array of size 4096 (4Kb), we leave starting 200, and start from 201rth index, the  200 spaces are used by the Chip8 itself, rest is left to be used by the program.

### Processing
- Chip 8 has only 37 different commands, which we will manually define and translate the working in C++ language.
- For example code 8XYN represents addition, so we will intercept this instruction code and do the job(addition), according to the moder computer and return the ans, tricking software into thinking that it got its answer processes from the CPU.

## Flow
> INPUT: Machine code ment for Chip8 -> MEMORY: store in modern data structures -> PROCESS: intercept the instruction codes and then process and return result -> OUTPUT: use a screen to display basic graphics with the calculated results

# retrival of instructions

## A brief on Hexadecimal
- 0x is used to represent a Hexadecimal number, each hex char is 4 bits.
- A nibble(4 bits) gets translated into one hexadecimal number.

## MORE on Memory & processing
- we receive machine code, we parse it, chop up the all the instruction, into two byte groups,
- These two bute groups, indicate one instruction.
- We create a big switch case, where we map these instruction with our memory.

## Retrival of specific parts of instruction  (Bitwise &)
- Bitwise AND only keeps the vales when mask has F in it
- Lets say 0xA22A is an instruction to get the last four letter we take a bitwise AND with mask 0XFFFF 

## Sliding the chars (Bitwise >>) 
- Now that we have the required character/Nibble out of a instruction code, we need to eliminate the extra 0 values.
- We do this with right shift(>>), we simply do >> 4 (4, as a hex is of 4 bits) for removing a single zeron on the right.
- 0xA000 >> 12 = 0xA

> so the bottom line is, we take a Nibble out of the instruction code, this single char tell us about the kind of operation we need to perform for this instruction code. We look for it in our switch statement, if ww have it then we execute it.

## Some Opcode
- 0x1NNN -> 0x1 tells us that its a code for Jump to address, the addred is defined by NNN

- 0x1 -> Jumping in memory address
- 0x8 -> ALU
- 0xE -> Keyboard controls, used to check key presses
- 0xF -> Hardware timers, Delay Timer, Sound Timer, bulk memory operations
# CHIP-8 Emulator
- Emulator are basically software layers to trick other software into thinking that they are being run on another computer, the one that they support natively
- Here Chip 8 emulator will trick some codes/softwares which were originally created for Chip 8, to think that they are running on the chip 8, but in reality they will be running on modern computers.

![img.png](img.png)
![img2.png](img2.png)
![demo.gif](demo.gif)


### Want to dive into the architecture, memory management, and opcode implementation? **[Check out the Technical Details](TechnicalDetails.md)**.

## Features
* Accurate opcode implementation based on cowgod's reference.
* Cross-platform support (Windows, macOS, (haven't tested the build on linux yet)).
* Automated CI pipeline for release builds.

## Running the Emulator

Download the latest release for your OS from the [Releases](https://github.com/ashish757/Chip8-Emulator/releases) tab.
### windows
`Windows defender` will block the execution, ignore the warning and click on `run anyway`.

### macOS
Apple will flag my executable, as unsafe to overcome this apple requires $99 to get verified, follow the below steps to run the application anyway -
1. Unzip and open the executable file inside.
2. now open `System Settings` and go to `privacy and security` then scroll down you will se option to `open anyway`, sorry I don't really have $99

### Loading ROMs
You can load any .ch8 ROM file, simply click on the IMPORT ROM button and select a valid .ch8 file, then click on the launch button to play.

## How to control the game?
The original CHIP-8 used a 16-key hex keypad.
This is mapped to your modern keyboard, use these keys to give input and control the games

<table>
  <thead>
    <tr>
      <th colspan="4" align="center">CHIP-8 Hex Keypad ➔ <kbd>Modern Keyboard</kbd></th>
    </tr>
  </thead>
  <tbody align="center">
    <tr>
      <td><b>1</b> ➔ <kbd>1</kbd></td>
      <td><b>2</b> ➔ <kbd>2</kbd></td>
      <td><b>3</b> ➔ <kbd>3</kbd></td>
      <td><b>C</b> ➔ <kbd>4</kbd></td>
    </tr>
    <tr>
      <td><b>4</b> ➔ <kbd>Q</kbd></td>
      <td><b>5</b> ➔ <kbd>W</kbd></td>
      <td><b>6</b> ➔ <kbd>E</kbd></td>
      <td><b>D</b> ➔ <kbd>R</kbd></td>
    </tr>
    <tr>
      <td><b>7</b> ➔ <kbd>A</kbd></td>
      <td><b>8</b> ➔ <kbd>S</kbd></td>
      <td><b>9</b> ➔ <kbd>D</kbd></td>
      <td><b>E</b> ➔ <kbd>F</kbd></td>
    </tr>
    <tr>
      <td><b>A</b> ➔ <kbd>Z</kbd></td>
      <td><b>0</b> ➔ <kbd>X</kbd></td>
      <td><b>B</b> ➔ <kbd>C</kbd></td>
      <td><b>F</b> ➔ <kbd>V</kbd></td>
    </tr>
  </tbody>
</table>


## Building from Source

To build this project locally, make sure you have a C++ compiler and cmake on your system.

```bash
git clone [https://github.com/ashish757/Chip8-Emulator.git](https://github.com/ashish757/Chip8-Emulator.git)
cd chip8-emulator
mkdir "build"
cd build
cmake ..
cmake --build
```

## The Final Boss: CI & Distribution
Getting the code to compile was only half the battle. Automating the distribution pipeline via Continuous Integration took countless iterations to resolve linker and config errors.

- while implementing ```Continuous Integration``` i did countless iterations solving dumb errors, adjusting platform depended configs and so on...
- but results are just as fin, now automatic releases are created for every tag.


# Credits
1. cowgod's  Chip 8 technical reference http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
2. The preloaded Chip8 ROMs are from kripod : https://github.com/kripod/chip8-roms
3. used Gemini to simplify some of the op codes and memory management, which were not clear in cowgod's blog

## AI
- Used JetBrain Clion's Auto completion.
- The UI(not the graphics engine) was created with the help of copilot and gemini, I learnt about Immediate Mode GUI, but took AI help, as my main goal was chip8, not UX.



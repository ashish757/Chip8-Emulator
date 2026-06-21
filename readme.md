# CHIP-8 Emulator



### Want technical details of the project? [checkout notes](TechnicalDetails.md)


## The Final Boss: CI & Distribution
Getting the code to compile was only half the battle. Automating the distribution pipeline via Continuous Integration took countless iterations to resolve linker and config errors.
* **Windows "DLL Hell":** Windows constantly threw missing DLL and pathing errors. Because the development environment was macOS, debugging required pinging a non-tech friend with new `.exe` files after every build.
* **Apple Gatekeeper:** macOS actively blocks applications from unverified developers unless the $99/yr Apple Developer fee is paid. To bypass this without paying Tim Cook, users must manually override security settings, though building a standard `.app` bundle structure helps tame Gatekeeper slightly.




# Credits
1. cowgod's  Chip 8 technical reference http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
2. used Gemini to simplify some of the op codes and memory management, which were not clean in cowgod's blog




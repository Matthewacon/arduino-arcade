# arduino-arcade
An implementation of the games and cellular automata in [turing-box](https://github.com/Matthewacon/turing-box) over serial and an LED array.

## Requirements
 - Arduino Nano v3
 - avr-gcc (not avr-core)
 - Make 4.2.1+
 - gcc 8.2.1+
 - BusyBox or Arduino Studio (for interactive stty)
 - A terminal that supports escape codes (Arduino Studio's serial console does not)
 
## Cloning
```bash
git clone --depth 1 --recurse-submodules https://github.com/Matthewacon/arduino-arcade
```
 
## Building
```bash
make clean upload
```

**Note 1:** If you are running this code on a different board, you will have to change the `BOARD_TAG`, `BOARD_SUB` and `AVRDUDE_ARD_BAUDRATE` properties in the [`Makefile`](https://github.com/Matthewacon/arduino-arcade/blob/master/Makefile) to match your board's configuration.

**Note 2:** If you are running this code on a board with less than 2K of SRAM, you may experience undefined behaviour as you run out of memory and the heap overwrites the stack. As a rule of thumb, the amount of memory required to run the program is roughly equal to: `STATIC_DATA` + [`WIDTH`](https://github.com/Matthewacon/arduino-arcade/blob/master/src/main.cpp#L13) * (2 * [`LENGTH`](https://github.com/Matthewacon/arduino-arcade/blob/master/src/main.cpp#L12) + 1) + 14 bytes. Thus to run on a platform with less SRAM, you'll have to tweak both the [`LENGTH`](https://github.com/Matthewacon/arduino-arcade/blob/master/src/main.cpp#L12) and [`WIDTH`](https://github.com/Matthewacon/arduino-arcade/blob/master/src/main.cpp#L13) values in order to fit within your memory constraints.

## Running
#### Option 1: Arduino Studio
Use the serial console built into Arduino Studio.

#### Option 2: Over STTY
This method requires BusyBox. To open an interactive session, run the following:
```shell
busybox microcom -t BAUD /dev/ttyUSBX
```
where `BAUD` is the rate specified in [your setup](https://github.com/Matthewacon/arduino-arcade/blob/master/src/main.cpp#L101) and `X` is the device number for your arduino's USB interface (either `/dev/ACMX` or `/dev/USBX`).

## Usage
All commands are sent over serial:

| Command | Description |
| :--- | :---|
| `Conway` | Switch the current game to [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Rules). |
| `AntiConway` | Switch the current game to [AntiConway](https://github.com/Matthewacon/turing-box/blob/master/include/anti_conway.h). |
| `Ulam` | Switch the current game to [Ulam-Warburton's Automaton](https://en.wikipedia.org/wiki/Ulam%E2%80%93Warburton_automaton). |
| `AntiUlam` | Switch the current game to [Anti-Ulam-Warburton's Automaton](https://github.com/Matthewacon/turing-box/blob/master/include/anti_ulam.h). |
| `Langton` | Switch the current game to [Langton's Ant](https://en.wikipedia.org/wiki/Langton's_ant#Rules). |
| `Pong` | Switch the current game to [Pong](https://en.wikipedia.org/wiki/Pong). |
| `Text` | Switch the current game to [Text Scrolling](https://github.com/Matthewacon/turing-box/blob/master/include/text.h). |
| `pause` | Pause the game. |
| `resume` | Resume the game. |
| `print` | Print the current board layout. |
| `clear` | Clear the serial console. | 
| `clear board` | Clear the board with `false`. |
| `random` | Fill the board with random values. |
| `set x y [TRUE | FALSE]` | Set the cell at `(x, y)`, where both `x` and `y` are positive integers, to either `TRUE` or `FALSE`. |
| `free` | Print the amount of free memory in bytes. |

**Note:** Changes between game modes, with the exception of `Text` and `Pong` will start with the state of the board from the last game mode, unless cleared or randomized beforehand.

## License
This project is licensed under the [M.I.T. License](https://github.com/Matthewacon/arduino-arcade/blob/master/LICENSE).

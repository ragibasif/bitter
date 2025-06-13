# Bitter Interpreter in C

A C implementation of the Bitter esoteric programming language - a minimalist
`Brainfuck` variant that operates on single-bit memory cells.

["Like Brainfuck - only much, much worse." - User:DMC, creator of Bitter](https://esolangs.org/wiki/Bitter)

## About Bitter

Bitter is a Turing-complete esoteric programming language created by [User:DMC](https://esolangs.org/wiki/User:DMC)
that operates on single-bit memory cells.

Key features:

- 1-bit memory cells
- Only 4 core commands: `<`, `>`, `(`, `)`
- 2 interpreter commands: `!`, `#`
- Infinite, unbounded memory tape (positive direction only)
- Memory initialized to 0

## Language Commands

| Command | Description                                                |
| ------- | ---------------------------------------------------------- |
| `>`     | Move pointer right by 1 and invert the bit                 |
| `<`     | Move pointer left by 1 and invert the bit                  |
| `(`     | If current bit is 1, continue; else jump past matching `)` |
| `)`     | Jump back to matching `(`                                  |
| `!`     | (Debug) Dump memory from 0 to highest data pointer reached |
| `#`     | (Debug) Pause execution and dump memory                    |

## Examples

## Installation

1. Clone this repository:

   ```shell
   git clone https://github.com/ragibasif/bitter.git
   cd bitter
   ```

2. Run `make`

   ```shell
   make run
   ```

## Acknowledgements

- [Bitter on Esolang Wiki](https://esolangs.org/wiki/Bitter)
- [User:DMC - Creator of Bitter](https://esolangs.org/wiki/User:DMC)

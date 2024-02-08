# Turing-Machine-Simulator
__________________________

This project implements a Turing Machine in the language C. The Turing Machine consists of:
1. An infinite tape, divided into cells
2. A read/write head which traverses along the tape, capable of reading the current cell, writing a new value in the current cell, and moving left or right
3. A state register
4. A finite table of instructions which, given the current state of the machine, and the value in the tape cell currently being read, retrieves an instruction which tells the machine to:
- Write some (possibly the same) item into the cell
- Move the head left or right one cell
- Change to some (possibly the same) state

The program should work for any input file which represents a Turing Machine in this format, with any number of states.







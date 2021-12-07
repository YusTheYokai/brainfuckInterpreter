# Brainfuck Interpreter written in C
## Build
A C Compiler will be needed for compiling. Using GCC, this can be done such as:

    gcc -o bfi main.c logging.c

## Run
The command structure is build up in the following way:

    bfi [relativeFilePath] {flags}

### Flags
Following flags can be set:

    -c: Prints the read code with syntax highlighting
    -r: Prints the brainfuck output in rainbow colors

### Examples
The folder brainfuckFiles contains examples, e.g. Hello World and Fibonacci.

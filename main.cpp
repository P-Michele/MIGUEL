#include <iostream>
#include <cstdint>
#include <cstring>
#include <stack>

using namespace std;

uint8_t memory[2048];
int pointer;

/* Executes a token from the program */
void execute_command(const char token, const string& program, int& programCounter) {
    switch(token) {
        case 'M':        //Moves pointer to right
            if (pointer < sizeof(memory) - 1)
                pointer++;
            break;
        case 'I':        //Increments current memory cell value
            memory[pointer]++;
            break;
        case 'G':        //Moves pointer to left
            if (pointer > 0)
                pointer--;
            break;
        case 'U':        //Decrements current memory cell value
            memory[pointer]--;
            break;
        case 'E':        //Prints a cell
            putchar(memory[pointer]);
            break;
        case 'L':        // Loop Start
            if (memory[pointer] == 0) {
                int count = 1;
                while (count > 0 && programCounter < program.length() - 1) {
                    programCounter++;
                    if (program[programCounter] == 'L')
                        count++;
                    else if (program[programCounter] == '!')
                        count--;
                }
            }
            break;
        case '!':       // Loop End
            if (memory[pointer] != 0) {
                int count = 1;
                while (count > 0 && programCounter > 0) {
                    programCounter--;
                    if (program[programCounter] == '!')
                        count++;
                    else if (program[programCounter] == 'L')
                        count--;
                }
            }
            break;
        case '?':       // Takes input
            {
                if (const int input = fgetc(stdin); input != EOF) {
                    memory[pointer] = static_cast<uint8_t>(input);
                }
            }
            break;
        default:
            break;
    }
}

/* Executes a valid MIGUEL program */
void execute(const string& program) {
    int programCounter = 0;
    pointer = 0;

    while (programCounter < program.length()) {
        execute_command(program[programCounter], program, programCounter);
        programCounter++;
    }
}

/* Function that permits to check if loops are balanced */
bool validate(const string& program) {
    stack<char> loopStack;
    for (char c : program) {
        if (c == 'L')
            loopStack.push(c);
        else if (c == '!') {
            if (loopStack.empty())
                return false;
            loopStack.pop();
        }
    }
    return loopStack.empty();
}

int main() {
    memset(memory, 0, sizeof(memory));
    //Hello World Program
    string program = "IIIIIIIIILMIIIIIIIIIMIIIIIIIMIIIMGGGGULEMIIIEMIIIEMIIIIIIEEEMUUUIIIIIIIIEMIIIUUUUUUUUUIEMIIIEMIMIE";
    if (!validate(program)) {
        cout << "Error: Unbalanced loops in program!" << endl;
        return 1;
    }

    execute(program);
    return 0;
}
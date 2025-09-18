#include <iostream>
#include <cstdint>
#include <cstring>
#include <stack>
#include <cstdio>

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
            putchar(static_cast<char>(memory[pointer]));
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

    //Hello World in MIGUEL obtained from brainfuck
    string program = "IIIIIIIIII"      // ++++++++++        cell[0]=10
    "L"               // [
    "MIIIIIII"        // >+++++++          cell[1]+=7
    "MIIIIIIIIII"     // >++++++++++       cell[2]+=10
    "MIII"            // >+++              cell[3]+=3
    "MI"              // >+                cell[4]+=1
    "GGGG"            // <<<<
    "U"               // -
    "!"               // ]

    "MIIE"            // >++.              'H' (72)
    "MIE"             // >+.               'e' (101)
    "IIIIIIIE"        // +++++++.           'l' (108)
    "E"               // .                 'l'
    "IIIE"            // +++.              'o' (111)
    "MIIE"            // >++.              ' ' (32)

    "GGIIIIIIIIIIIIIIIE" // <<+++++++++++++++. 'W' (15 I)
    "ME"              // >.                'o'
    "IIIE"            // +++.              'r'
    "UUUUUUE"         // ------.           'l'
    "UUUUUUUUE"       // --------.         'd'
    "MIE"             // >+.               '!'
    "ME";             // >.                '\n'

    if (!validate(program)) {
        cout << "Error: Unbalanced loops in program!" << endl;
        return 1;
    }

    cout << "Executing MIGUEL" << endl;
    execute(program);
    cout << endl;

    return 0;
}

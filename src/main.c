#include "program/program.h"


int main(void)
{
    Program* program = start_program();
    main_loop(program);
    quit_program(program);
}

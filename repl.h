#ifndef REPL_H
#define REPL_H

#include <setjmp.h>

extern jmp_buf repl_start;

void reset_repl(std::string msg);
void repl();

#endif

#include <iostream>
#include <stdlib.h>
#include <signal.h>

#include "lexer.h"
#include "sexp.h"
#include "io.h"
#include "defaults.h"
#include "repl.h"

void handler(int temp)
{
  // std::cerr << "hello\n";
  reset_repl();
}

int main(int argc, char **argv)
{
  signal(SIGINT, handler);
  defaults();
  the_empty_list = new Sexp{false};

  repl();
}

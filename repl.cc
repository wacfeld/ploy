#include <iostream>
#include <vector>

#include "lexer.h"
#include "sexp.h"
#include "repl.h"
#include "io.h"

jmp_buf repl_start;
const int max_depth = 1000;
int rec_depth;

void reset_repl(std::string msg)
{
  std::cerr << msg << std::endl;
  longjmp(repl_start, 1);
}

void reset_repl()
{
  longjmp(repl_start, 1);
}

void repl()
{
  setjmp(repl_start);

  rec_depth = 0;
  
  while(true) {
    std::vector<Token> toks = read_tokens(std::cin);
    if(toks.empty()) {
      break;
    }

    toks = expand_quote(toks);
    Sexp *e = get_sexp(toks);
    Sexp *f = eval(e, {});
    std::cout << *f << std::endl;
  }
  
  cleanup();
  std::cerr << "bye\n";
}

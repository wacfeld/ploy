#include "defaults.h"
#include "forms.h"

// syntactic forms are quote, if, begin, cond, etc.
std::set<std::string> forms {
  "quote", "if", "define",
};

// true if e is syntactic form
bool is_form(Sexp *e)
{
  if(!e->atom)
    return false;

  if(e->a.type != SYMBOL)
    return false;

  return forms.count(e->a.symb);
}

void check_length(const std::string &f, Sexp *e, int len, bool ge)
{
  int ll = list_len(e);
  bool ok = ge ? (ll >= len) : (ll == len);
  if(!ok) {
    std::cerr << f << " requires " << len << " argument(s); received " << ll << std::endl;
    longjmp(repl_start, 1);
  }
}

Sexp *eval_form(Sexp *e, std::map<std::string,Sexp*> env)
{
  std::string &f = e->car->a.symb;

  if(f == "define") {
    check_length(f, e->cdr, 2);
    Sexp *a = e->cdr->car;
    Sexp *b = e->cdr->cdr->car;
    
    if(a->atom) {
      if(a->a.type == SYMBOL) {
        b = eval(b, env);
        bind(a->a.symb, b);
      }

      else {
        reset_repl("cannot define non-symbol");
      }
    }

    else {
      reset_repl("function definitions not supported yet");
    }

    return a;
  }

  // else if(f == "lambda") {
  //   check_length(f, e->cdr, 2);
  //   Sexp *formals = e->cdr->car;
  //   Sexp *body = e->cdr->cdr->car; // TODO allow multiple body expressions
    
  //   if(islist(formals)) {
      
  //   }
  //   else {
  //     reset_repl("non-list formals not supported yet");
  //   }
  // }

  else if(f == "quote") {
    check_length(f, e->cdr, 1);

    return index(e, 2);
  }

  else if(f == "if") {
    check_length(f, e->cdr, 3);

    // evaluate condition
    Sexp *args = e->cdr;
    Sexp *cond = eval(index(args, 1), env);
    bool truth = eval_truth(cond);
    
    // evaluate first branch
    if(truth) {
      return eval(index(args, 2), env);
    }

    // evaluate second branch
    else {
      return eval(index(args, 3), env);
    }
  }

  else {
    std::cerr << "don't recognize syntactic form " << f << std::endl;
    // exit(1);
    longjmp(repl_start, 1);
  }
}

#include "defaults.h"
#include "forms.h"

// syntactic forms are quote, if, begin, cond, etc.
std::set<std::string> forms {
  "quote", "if", "define", "lambda",
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

Sexp *eval_form(Sexp *e, const std::map<std::string,Sexp*> &env)
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

  else if(f == "lambda") {
    check_length(f, e->cdr, 2);
    Sexp *formals = e->cdr->car;
    Sexp *body = e->cdr->cdr->car; // TODO allow multiple body expressions

    Sexp *L = new Sexp{true};
    L->a.type = PROCEDURE;
    L->a.proc.prim = false;
    L->a.proc.formals = formals;
    // a formal is one of:
    // - a symbol
    // - the empty list
    // - a pair of symbol and formal
    // e.x. (), (a), (a b), a, (a . b), (a b . c) are all formals
    
    L->a.proc.env = env; // copy current local env
    L->a.proc.body = body;
    
    // if(islist(formals)) {
    //   // write formals into L->a.proc.formals
    //   for(Sexp *it = formals; !isempty(it); it = it->cdr) {
    //     Sexp *f = it->car;
    //     if(!issymbol(f)) {
    //       reset_repl("formals list contains non-symbol");
    //     }

    //     L->a.proc.formals.push_back(f->a.symb);
    //   }
    // }
    // else {
    //   reset_repl("non-list formals not supported yet");
    // }
    
    return L;
  }

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

#include "defaults.h"
#include "lexer.h"
#include "proc.h"
#include "forms.h"

void defaults()
{
  // bind("length", list_len);
  bind("cons", cons);
  bind("car", car);
  bind("cdr", cdr);
  bind("null?", null);
  
  bind("not", naught);
  
  bind("=", num_eq);
  bind("+", add);
  bind("-", sub);
}

Sexp *cons(Sexp *args)
{
  check_length(__func__, args, 2);
  Sexp *a = args->car;
  Sexp *b = args->cdr->car;
  
  Sexp *e = new Sexp{false};
  e->car = a;
  e->cdr = b;
  return e;
}

Sexp *car(Sexp *args)
{
  check_length(__func__, args, 1);
  Sexp *a = args->car;
  if(a->atom) {
    std::cerr << "car given atom\n";
    longjmp(repl_start, 1);
  }
  if(isempty(a)) {
    std::cerr << "car given ()\n";
    longjmp(repl_start, 1);
  }
  return a->car;
}

Sexp *cdr(Sexp *args)
{
  check_length(__func__, args, 1);
  Sexp *a = args->car;
  if(a->atom) {
    std::cerr << "cdr given atom\n";
    longjmp(repl_start, 1);
  }
  if(isempty(a)) {
    std::cerr << "cdr given ()\n";
    longjmp(repl_start, 1);
  }
  return a->cdr;
}

Sexp *null(Sexp *args)
{
  check_length(__func__, args, 1);
  Sexp *a = args->car;

  return make_bool(isempty(a));
}

// "not" is a C++ keyword
Sexp *naught(Sexp *args)
{
  check_length(__func__, args, 1);
  Sexp *a = args->car;
  bool truth = !eval_truth(a);
  return make_bool(truth);
}

void check_arith_usage(const std::string &name, Sexp *args, int nargs)
{
  check_length(name, args, nargs);
  while(!isempty(args)) {
    Sexp *a = args->car;
    if(!isnum(a)) {
      std::cerr << "usage: (" << name;
      for(int i = 0; i < nargs; i++) {
        std::cerr << " NUM";
      }
      std::cerr << ")" << std::endl;
      reset_repl();
    }
    
    args = args->cdr;
  }
}

std::vector<int> get_ints(Sexp *args, int nints)
{
  std::vector<int> ret;
  while(!isempty(args)) {
    Sexp *a = args->car;
    ret.push_back(a->a.num);
    args = args->cdr;
  }

  return ret;
}

// =
Sexp *num_eq(Sexp *args)
{
  check_arith_usage(__func__, args, 2);
  std::vector<int> ints = get_ints(args, 2);
  bool truth = (ints[0] == ints[1]);
  return make_bool(truth);
}

Sexp *add(Sexp *args)
{
  check_arith_usage(__func__, args, 2);
  std::vector<int> ints = get_ints(args, 2);
  return make_num(ints[0]+ints[1]);
}

Sexp *sub(Sexp *args)
{
  check_arith_usage(__func__, args, 2);
  std::vector<int> ints = get_ints(args, 2);
  return make_num(ints[0]-ints[1]);
}

Sexp *mul(Sexp *args)
{
  check_arith_usage(__func__, args, 2);
  std::vector<int> ints = get_ints(args, 2);
  return make_num(ints[0]*ints[1]);
}

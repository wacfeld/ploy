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
  bind("list?", list);
  bind("length", length);
  
  bind("not", naught);
  
  bind("=", num_eq);
  bind("+", add);
  bind("-", sub);
  bind("*", mul);
  bind("/", div);
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

Sexp *list(Sexp *args)
{
  check_length(__func__, args, 1);
  Sexp *e = args->car;
  return make_bool(islist(e));
}

Sexp *null(Sexp *args)
{
  check_length(__func__, args, 1);
  Sexp *a = args->car;

  return make_bool(isempty(a));
}

Sexp *length(Sexp *args)
{
  check_length(__func__, args, 1);
  Sexp *a = args->car;
  return make_num(list_len(a));
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

bool all_nums(Sexp *args)
{
  if(!islist(args)) {
    reset_repl("allnums(): passed non-list");
  }

  while(!isempty(args)) {
    Sexp *a = args->car;
    if(!isnum(a)) {
      return false;
    }
    args = args->cdr;
  }

  return true;
}

std::vector<int> get_ints(Sexp *args)
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
  // check_arith_usage(__func__, args, 2);
  if(!all_nums(args)) {
    reset_repl(std::string{__func__} + std::string{" passed non-numbers"});
  }
  std::vector<int> ints = get_ints(args);
  bool eq = true;
  if(!ints.empty()) {
    for(int n : ints) {
      if(n != ints[0]) {
        eq = false;
        break;
      }
    }
  }
  return make_bool(eq);
}

Sexp *add(Sexp *args)
{
  check_arith_usage(__func__, args, 2);
  std::vector<int> ints = get_ints(args);
  return make_num(ints[0]+ints[1]);
}

Sexp *sub(Sexp *args)
{
  check_arith_usage(__func__, args, 2);
  std::vector<int> ints = get_ints(args);
  return make_num(ints[0]-ints[1]);
}

Sexp *mul(Sexp *args)
{
  check_arith_usage(__func__, args, 2);
  std::vector<int> ints = get_ints(args);
  return make_num(ints[0]*ints[1]);
}

Sexp *div(Sexp *args)
{
  check_arith_usage(__func__, args, 2);
  std::vector<int> ints = get_ints(args);
  if(ints[1] == 0) {
    reset_repl("division by zero");
  }
  return make_num(ints[0]/ints[1]);
}

#include "defaults.h"
#include "lexer.h"
#include "proc.h"
#include "forms.h"

typedef unsigned long ulong;

void defaults()
{
  // bind("length", list_len);
  bind("cons", cons);
  bind("car", car);
  bind("cdr", cdr);
  bind("null?", null);
  bind("list?", listp);
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

Sexp *listp(Sexp *args)
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

void check_arith_usage(const std::string &name, Sexp *args)
{
  if(!all_nums(args)) {
    reset_repl(name + std::string{" passed non-numbers"});
  }
}

void check_arith_usage(const std::string &name, Sexp *args, int nargs)
{
  check_length(name, args, nargs);
  check_arith_usage(name, args);
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
  check_arith_usage(__func__, args);
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
  check_arith_usage(__func__, args);
  std::vector<int> ints = get_ints(args);
  int sum = 0;
  for(int n : ints) {
    sum += n;
  }
  return make_num(sum);
}

Sexp *sub(Sexp *args)
{
  check_arith_usage(__func__, args);
  std::vector<int> ints = get_ints(args);
  
  if(ints.empty()) {
    reset_repl(std::string{__func__} + std::string{" passed 0 arguments"});
    exit(1);
  } else if(ints.size() == 1) {
    return make_num(-ints[0]);
  } else {
    int d = ints[0];
    for(ulong i = 1; i < ints.size(); i++) {
      d -= ints[i];
    }
    return make_num(d);
  }
}

Sexp *mul(Sexp *args)
{
  check_arith_usage(__func__, args);
  std::vector<int> ints = get_ints(args);
  int prod = 1;
  for(int n : ints) {
    prod *= n;
  }
  return make_num(prod);
}

Sexp *div(Sexp *args)
{
  check_arith_usage(__func__, args);
  std::vector<int> ints = get_ints(args);
  
  if(ints.empty()) {
    reset_repl(std::string{__func__} + std::string{" passed 0 arguments"});
    exit(1);
  } else if(ints.size() == 1) {
    if(ints[0] == 0) {
      reset_repl("division by 0");
    }
    return make_num(1/ints[0]);
  } else {
    int d = ints[0];
    for(ulong i = 1; i < ints.size(); i++) {
      if(ints[i] == 0) {
        reset_repl("division by 0");
      }
      d /= ints[i];
    }
    return make_num(d);
  }
}

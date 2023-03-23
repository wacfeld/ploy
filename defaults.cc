#include "defaults.h"
#include "lexer.h"
#include "proc.h"
#include "forms.h"

void defaults()
{
  // bind("length", list_len);
  bind("+", add);
  bind("cons", cons);
  bind("car", car);
  bind("cdr", cdr);
  bind("null?", null);
  bind("not", naught);
  bind("=", num_eq);
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
  if(a == the_empty_list) {
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
  if(a == the_empty_list) {
    std::cerr << "cdr given ()\n";
    longjmp(repl_start, 1);
  }
  return a->cdr;
}

Sexp *null(Sexp *args)
{
  check_length(__func__, args, 1);
  Sexp *a = args->car;

  return make_bool(a == the_empty_list);
}

// "not" is a C++ keyword
Sexp *naught(Sexp *args)
{
  check_length(__func__, args, 1);
  Sexp *a = args->car;
  bool truth = !eval_truth(a);
  return make_bool(truth);
}

// =
Sexp *num_eq(Sexp *args)
{
  check_length(__func__, args, 2);
  Sexp *a = args->car;
  Sexp *b = args->cdr->car;
  if(!isnum(a) || !isnum(b)) {
    std::cerr << "usage: (= NUM NUM)\n";
    longjmp(repl_start, 1);
  }

  bool truth = (a->a.num == b->a.num);
  return make_bool(truth);
}

Sexp *add(Sexp *args)
{
  static std::string usage = "usage: (add NUM NUM)";
  // if(list_len(args) != 2) {
  //   std::cerr << usage << std::endl;
  //   // exit(1);
  //   longjmp(repl_start, 1);
  // }
  check_length(__func__, args, 2);

  Sexp *a = args->car;
  Sexp *b = args->cdr->car;

  if(!isnum(a) || !isnum(b)) {
    std::cerr << usage << std::endl;
    longjmp(repl_start, 1);
  }

  int sum = a->a.num + b->a.num;
  Sexp *e = new Sexp{true};
  e->a.type = NUMBER;
  e->a.num = sum;
  
  return e;
}

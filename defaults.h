#ifndef DEFAULTS_H
#define DEFAULTS_H

#include "sexp.h"
#include "repl.h"

// int list_len(Sexp *e);

void defaults();

Sexp *cons(Sexp *args);
Sexp *car(Sexp *args);
Sexp *cdr(Sexp *args);
Sexp *null(Sexp *args);

Sexp *naught(Sexp *args);

void check_arith_usage(const std::string &name, Sexp *args, int nargs)
Sexp *num_eq(Sexp *args);
Sexp *add(Sexp *args);

#endif

#include "proc.h"
#include "sexp.h"

std::map<std::string, Sexp*> global_env;

// add binding to global environment
void bind(const std::string &name, Sexp *e)
{
  global_env[name] = e;
}

void bind(const std::string &name, Sexp *(*f)(Sexp *))
{
  Sexp *e = new Sexp{true};
  e->a.type = PROCEDURE;
  e->a.proc.prim = true;
  e->a.proc.f = f;

  bind(name, e);
}

void unbind(const std::string &name)
{
  global_env.erase(name);
}

bool isproc(Sexp *e)
{
  return e->atom && (e->a.type == PROCEDURE);
}

// read arguments into env according to formals
void read_args(std::map<std::string, Sexp*> &env, Sexp *form, Sexp *args)
{
  while(1) {
    // done reading
    if(isempty(form)) {
      // make sure no leftover arguments
      if(!isempty(args)) {
        reset_repl("too many arguments");
      }
      break;
    }

    // normal argument reading
    else if(islist(form)) {
      // make sure car is symbol
      if(!issymbol(form->car)) {
        reset_repl("non-symbol formal");
      }
      // make sure args still exist
      if(isempty(args)) {
        reset_repl("not enough arguments");
      }

      // write to env
      std::string &name = form->car->a.symb;
      env[name] = args->car;
    }

    // gobble gobble
    else if(issymbol(form)) {
      // give rest of args to the symbol
      std::string &name = form->a.symb;
      env[name] = args;
      break;
    }

    else {
      reset_repl("invalid formal(s)");
    }

    // move it forward
    args = args->cdr;
    form = form->cdr;
  }
}

// call procedure proc on list of arguments args
Sexp *call(const Proc &proc, Sexp *args)
{
  // primitive procedure call
  if(proc.prim) {
    return (proc.f)(args);
  }

  // compound procedure call
  else {
    // std::cerr << "non-primitive procedure call hasn't been implemented\n";
    // exit(1);
    // longjmp(repl_start, 1);

    // make copy of environment
    std::map<std::string, Sexp*> env = proc.env;
    
    // std::cerr << "reading args\n";
    // read args into env
    read_args(env, proc.formals, args);
    // std::cerr << "done reading args\n";

    // evaluate body
    return eval(proc.body, env);
  }
}

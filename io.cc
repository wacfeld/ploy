#include "io.h"

std::ostream &operator<<(std::ostream &out, Proc &p)
{
  if(p.prim) {
    // out << p.f;
    out << "[builtin ";
    printf("%p", p.f);
    out << "]";
  } else {
    // std::cerr << "non-primitive procedure operator<< not supported\n";
    out << "[compound ";
    printf("%p", p.body);
    out << "]";
  }

  return out;
}

std::ostream &operator<<(std::ostream &out, Atom &a)
{
  if(a.type == NUMBER) {
    out << a.num;
  } else if(a.type == SYMBOL) {
    out << a.symb;
  } else if(a.type == BOOLEAN) {
    out << (a.boole ? "#t" : "#f");
  } else if(a.type == PROCEDURE) {
    // out << "[proc ";
    // printf("%p", a.proc.f);
    // out << "]";
    out << a.proc;
  }

  return out;
}

void flat_sexp(std::ostream &out, Sexp *e, bool open)
{
  if(e->atom) {
    if(!open) {
      out << ". ";
    }
    out << e->a << " ";
    if(!open) {
      out << ") ";
    }
  }

  else if(isempty(e)) {
    if(open) {
      out << "(";
    }
    out << ") ";
  }

  else {
    if(open) {
      out << "( ";
    }
    flat_sexp(out, e->car, true);
    flat_sexp(out, e->cdr, false);
  }
}

void put_sexp(std::ostream &out, Sexp *e, int indent)
{
  // indent
  for(int i = 0; i < indent; i++) {
    out << ' ';
  }
  
  if(isempty(e)) {
    out << "[/]" << std::endl;
  }

  else if(e->atom) {
    out << e->a << std::endl;
  }

  else {
    out << "[ ]" << std::endl;
    put_sexp(out, e->car, indent+2);
    put_sexp(out, e->cdr, indent+2);
  }
}

std::ostream &operator<<(std::ostream &out, Sexp &e)
{
  flat_sexp(out, &e);
  return out;
}

std::ostream &operator<<(std::ostream &out, std::vector<Token> &toks)
{
  for(Token &T : toks) {
    out << T << " ";
  }
  
  return out;
}

std::ostream &operator<<(std::ostream &out, Token t)
{
  // out << t.type << ' ';
  if(t.type == OPAR) {
    out << '(';
  } else if(t.type == CPAR) {
    out << ')';
  } else if(t.type == QUOT) {
    out << '\'';
  } else if(t.type == BOOL) {
    out << (t.b ? "#t" : "#f");
  } else if(t.type == IDENT) {
    out << t.i;
  } else if(t.type == NUM) {
    out << t.n;
  }

  return out;
}


std::ostream &operator<<(std::ostream &out, ttype t)
{
  if(t == BOOL) out << "BOOL";
  else if(t == IDENT) out << "IDENT";
  else if(t == NUM) out << "NUM";
  else if(t == OPAR) out << "OPAR";
  else if(t == CPAR) out << "CPAR";
  else if(t == QUOT) out << "QUOT";
  else if(t == NONE) out << "NONE";
  return out;
}


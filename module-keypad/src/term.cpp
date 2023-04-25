#include "term.h"

Term::Term()
{
  value = -1;
  next = 0;
}

Term::Term(short val)
{
  value = val;
  next = 0;
}

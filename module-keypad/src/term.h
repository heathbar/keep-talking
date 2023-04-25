#ifndef TERM_H
#define TERM_H

class Term
{
  public:
    short value;
    Term *next;

    Term();
    Term(short val);
};


#endif
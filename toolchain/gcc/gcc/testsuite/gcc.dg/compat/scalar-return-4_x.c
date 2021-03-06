#include "compat-common.h"

#define T(NAME, TYPE, INITVAL) 					\
TYPE g01##NAME;							\
								\
extern void init##NAME (TYPE *p, TYPE v);			\
extern void checkg##NAME (void);				\
extern TYPE test0##NAME (void);					\
extern TYPE test1##NAME (TYPE);					\
								\
void								\
check##NAME (TYPE x, TYPE v)					\
{								\
  if (x != v)							\
    DEBUG_CHECK							\
}								\
								\
void								\
testit##NAME (void)						\
{								\
  TYPE rslt;							\
  DEBUG_FPUTS (#NAME);						\
  DEBUG_FPUTS (" init: ");					\
  init##NAME (&g01##NAME,  1);					\
  checkg##NAME ();						\
  DEBUG_NL;							\
  DEBUG_FPUTS (#NAME);						\
  DEBUG_FPUTS (" test0: ");					\
  rslt = test0##NAME ();					\
  check##NAME (rslt, g01##NAME);				\
  DEBUG_NL;							\
  DEBUG_FPUTS (#NAME);						\
  DEBUG_FPUTS (" test1: ");					\
  rslt = test1##NAME (g01##NAME);				\
  check##NAME (rslt, g01##NAME);				\
  DEBUG_NL;							\
}

T(cc, _Complex char, (0,1))
T(cs, _Complex short, (1,2))
T(cf, _Complex float, (1.0,2.0))

#undef T

void
scalar_return_4_x ()
{
DEBUG_INIT

#define T(NAME) testit##NAME ();

T(cc)
T(cs)
T(cf)

DEBUG_FINI

if (fails != 0)
  abort ();

#undef T
}

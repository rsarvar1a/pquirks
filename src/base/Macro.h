
#ifndef PQ_MACRO_H_
#define PQ_MACRO_H_

#define stx(x) #x
#define stringify(x) stx(x)
#define inc(x) stringify(x/x.h)
#define rule(x) stringify(../x/x.h)

#endif


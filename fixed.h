// Code concerning working with fixed point numbers, specically 20.12 fixed
// point numbers.

#ifndef FIXED_H
#define FIXED_H

// 20.12 fixed because thats what the PS1 natively handles.
typedef int fixed;

#define itoF(n) ((n) << 12)
#define Fixed_Add(n, x) ((n) + (x))
#define Fixed_AddInt(n, x) ((n) + Fixed(x))
#define Fixed_MultIntF(n, x) ((n) * (x))
#define Fixed_DivInt(n, x) ((n) / (x))
#define Ftoi(n) ((n) >> 12)

#endif

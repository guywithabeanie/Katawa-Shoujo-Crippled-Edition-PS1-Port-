#ifndef ERROR_H
#define ERROR_H

#include "renderer.h"

typedef enum Status {
  Okay = 0,

  FileLoadError,
  MallocError,
  FileReadError,
  FileFindError,
  VRAMError
} Status;

static char *StatusStr[] = {"Task failed succesfully."
                            "Couldn't load file.",
                            "malloc() error.",
                            "Couldn't read file."
                            "File not found."};

#define ErrorCheck(n)                                                          \
  do {                                                                         \
    int status = (n);                                                          \
    if (status != Okay) {                                                      \
      while (1) {                                                              \
        FntPrint("Error at %s:%d : %s\n", __FILE__, __LINE__, StatusStr[n]);   \
        FntFlush(-1);                                                          \
      }                                                                        \
    }                                                                          \
  } while (0)

#endif

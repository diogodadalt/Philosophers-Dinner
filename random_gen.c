#include "random_gen.h"

int random_in_range(unsigned int min, unsigned int max) {
  return rand() % (max - min + 1) + min;
}
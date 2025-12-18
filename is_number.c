#include "is_number.h"

#include <stdlib.h>
#include <stdbool.h>

bool is_number(const char *str)
{
  // https://chatgpt.com/share/6943b789-d750-8006-b630-1321331af182

  if (!str || *str == '\0')
    return false;

  char *end;
  strtod(str, &end);

  return *end == '\0';
}
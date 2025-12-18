#include "assert.h"
#include <string.h>
#include <stdio.h>

#include "is_number.h"

int main()
{
  int test = 1;
  // int tests = 30;

  // printf("1..%i\n", tests);

  assert(is_number("number") == 0);
  printf("%d. [PASSED] 'number' is NOT a number\n", test++);

  assert(is_number("1") == 1);
  printf("%d. [PASSED] '1' is a number\n", test++);
  
  assert(is_number("23344.532") == 1);
  printf("%d. [PASSED] '23344.532' is a number\n", test++);

  assert(is_number("-3.14") == 1);
  printf("%d. [PASSED] '-3.14' is a number\n", test++);

  assert(is_number("1E9") == 1);
  printf("%d. [PASSED] '1E9' is a number\n", test++);

  assert(is_number("2.4E6") == 1);
  printf("%d. [PASSED] '2.4E6' is a number\n", test++);

  assert(is_number("2.4x") == 0);
  printf("%d. [PASSED] '2.4x' is NOT a number\n", test++);

  assert(is_number("+0.122") == 1);
  printf("%d. [PASSED] '+0.122' is a number\n", test++);

  
}
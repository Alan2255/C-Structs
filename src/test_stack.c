#include "stack.h"
#include "int.h"


int main() {

  Stack stack = stack_create(2, copy_int, destroy_int, compare_int, visit_int);

  int n;

  puts("Push 1");
  n = 1;
  stack_push(stack, &n);
  stack_print(stack);
  puts("");

  puts("Push 3");
  n = 3;
  stack_push(stack, &n);
  stack_print(stack);
  puts("");
  
  puts("Push 5");
  n = 5;
  stack_push(stack, &n);
  stack_print(stack);
  puts("");

  puts("Push 7");
  n = 7;
  stack_push(stack, &n);
  stack_print(stack);
  puts("");
  
  puts("Top");
  visit_int(stack_top(stack));
  puts("\n");

  puts("Pop");
  stack_pop(stack);
  stack_print(stack);
  puts("");

  puts("Pop");
  stack_pop(stack);
  stack_print(stack);
  puts("");

  puts("Pop");
  stack_pop(stack);
  stack_print(stack);
  puts("");


  stack_destroy(stack);

  puts("");
  return 0;
}
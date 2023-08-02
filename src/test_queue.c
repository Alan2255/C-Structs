#include "queue.h"
#include "int.h"


int main() {

  Queue queue = queue_create(copy_int, destroy_int, compare_int, visit_int);

  int n;

  puts("Push 1");
  n = 1;
  queue_push(queue, &n);
  queue_print(queue);
  puts("");

  puts("Push 3");
  n = 3;
  queue_push(queue, &n);
  queue_print(queue);
  puts("");
  
  puts("Push 5");
  n = 5;
  queue_push(queue, &n);
  queue_print(queue);
  puts("");

  puts("Push 7");
  n = 7;
  queue_push(queue, &n);
  queue_print(queue);
  puts("");
  
  puts("Top");
  visit_int(queue_top(queue));
  puts("\n");

  puts("Pop");
  queue_pop(queue);
  queue_print(queue);
  puts("");

  puts("Pop");
  queue_pop(queue);
  queue_print(queue);
  puts("");

  puts("Pop");
  queue_pop(queue);
  queue_print(queue);
  puts("");


  queue_destroy(queue);

  puts("");
  return 0;
}
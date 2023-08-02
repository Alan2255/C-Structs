#include "array.h"
#include "int.h"


int main() {


  Array array = array_create(10, copy_int, destroy_int, compare_int, visit_int);

  int n;

  puts("Write 1 at 0, 1, 2, 3, 4");
  n = 1;
  array_write(array, &n, 0);
  array_write(array, &n, 1);
  array_write(array, &n, 2);
  array_write(array, &n, 3);
  array_write(array, &n, 4);

  array_print(array);
  puts("");

  puts("Resize 2");
  array_resize(array, 2);

  array_print(array);
  puts("");

  puts("Resize 6");
  array_resize(array, 6);

  array_print(array);
  puts("");

  puts("Write 3 at 1");
  n = 3;
  array_write(array, &n, 1);
  array_print(array);
  puts("");

  puts("Insert 2 at 1");
  n = 2;
  array_insert(array, &n, 1);
  array_print(array);
  puts("");

  puts("Insert 9 at 5");
  n = 9;
  array_insert(array, &n, 5);
  array_print(array);
  puts("");

  puts("Insert 7 at 4");
  n = 7;
  array_insert(array, &n, 4);
  array_print(array);
  puts("");
  
  puts("Insert 100 at 4");
  n = 100;
  array_insert(array, &n, 4);
  array_print(array);
  puts("");

  puts("Delete at 5");
  array_delete(array, 5);
  array_print(array);
  puts("");

  puts("Delete at 5");
  array_delete(array, 5);
  array_print(array);
  puts("");

  puts("Insert -1 at 0");
  n = -1;
  array_insert(array, &n, 0);
  array_print(array);
  puts("");
  
  array_destroy(array);

  puts("");
  return 0;
}
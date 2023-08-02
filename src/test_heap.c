#include "heap.h"
#include "int.h"

int main() {
	
	BHeap bheap_max = bheap_create(10, MAX, copy_int, destroy_int, compare_int, visit_int);
	BHeap bheap_min = bheap_create(10, MIN, copy_int, destroy_int, compare_int, visit_int);
	
	int n;
	
	n = 1;
	bheap_add(bheap_max, &n);
	bheap_add(bheap_min, &n);
	
	n = 99;
	bheap_add(bheap_max, &n);
	bheap_add(bheap_min, &n);
	
	n = 45;
	bheap_add(bheap_max, &n);
	bheap_add(bheap_min, &n);
	
	n = 12;
	bheap_add(bheap_max, &n);
	bheap_add(bheap_min, &n);
	
	n = 70;
	bheap_add(bheap_max, &n);
	bheap_add(bheap_min, &n);
	
	puts("Heap max");
	bheap_print(bheap_max);
	puts("");
	
	puts("Heap min");
	bheap_print(bheap_min);
	puts("");puts("");
	
	bheap_pop(bheap_max);
	bheap_pop(bheap_min);
	
	puts("Heap max pop 1");
	bheap_print(bheap_max);
	puts("");

	puts("Heap min pop 1");
	bheap_print(bheap_min);
	puts("");puts("");
	
	bheap_pop(bheap_max);
	bheap_pop(bheap_min);
	
	puts("Heap max pop 2");
	bheap_print(bheap_max);
	puts("");
	puts("Heap min pop 2");
	bheap_print(bheap_min);
	puts("");
	
	bheap_destroy(bheap_max);
	bheap_destroy(bheap_min);
	
	puts("");
	return 0;
}
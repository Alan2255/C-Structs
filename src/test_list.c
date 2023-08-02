#include "list.h"
#include "int.h"

int main() {

	List list = list_create();
    
	int n;

	for (int i = 0; i < 10; i++) {
    	
		list = list_add(list, &i, copy_int);
	}
    
	list_print(list, visit_int);
    puts("");

	n = 100;
	list = list_insert(list, &n, 100, copy_int);

	list_print(list, visit_int);
    puts("");


	printf("Length: %i", list_length(list));

    list_destroy(list, destroy_int);

	puts("");
	return 0;
}
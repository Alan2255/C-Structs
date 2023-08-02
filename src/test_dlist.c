#include "list.h"
#include "int.h"

int main() {

	DList list = dlist_create();
    
	for (int i = 0; i < 10; i++) {
    	
		list = dlist_add(list, &i, FORDWARD, copy_int);
	}
    
	dlist_print(list, FORDWARD, visit_int);
    puts("");
	dlist_print(list, BACKWARD, visit_int);
    puts("");

	printf("Length: %i", dlist_length(list));

    dlist_destroy(list, destroy_int);

	puts("");
	return 0;
}
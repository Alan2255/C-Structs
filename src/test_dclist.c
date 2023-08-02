#include "list.h"
#include "int.h"

int main() {

    DCList list = dclist_create();
    
	for (int i = 0; i < 10; i++) {
    	
		list = dclist_add(list, &i, FORDWARD, copy_int);
	}

    dclist_print(list, FORDWARD, visit_int);
    puts("");
    dclist_print(list, BACKWARD, visit_int);
    puts("");

	printf("Length: %i", dclist_length(list));

    dclist_destroy(list, destroy_int);
	
	puts("");
	return 0;
}
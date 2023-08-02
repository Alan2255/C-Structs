#include "list.h"
#include "int.h"

int main() {

	CList list = clist_create();
    
	for (int i = 0; i < 10; i++) {
    	
		list = clist_add(list, &i, FORDWARD, copy_int);
	}
    
	clist_print(list, visit_int);
    puts("");

	printf("Length: %i", clist_length(list));

    clist_destroy(list, destroy_int);

	puts("");
	return 0;
}
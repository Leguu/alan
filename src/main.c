#include <stdio.h>
#include "flan.h"

int main() {
	struct flan *flan1 = flan_init_set(10, "20");
	struct flan *flan2 = flan_init_set(10, "21");

	printf("4: FLAN1: %s, FLAN2: %s\n", flan_as_str(flan1), flan_as_str(flan2));

	flan_sub(flan1, flan2);

	printf("5: FLAN1: %s, FLAN2: %s\n", flan_as_str(flan1), flan_as_str(flan2));

	flan_set(flan2, "500");
	flan_add(flan1, flan2);

	printf("6: FLAN1: %s, FLAN2: %s\n", flan_as_str(flan1), flan_as_str(flan2));
}

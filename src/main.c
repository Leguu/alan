#include <stdio.h>
#include "flan.h"

int main() {
	struct flan *flan1 = flan_init_set(10, "25");
	struct flan *flan2 = flan_init_set(10, "21");

	printf("actual: %d\n", flan_num_digits(flan1));

	printf("flan: %s\n", flan_as_str(flan1));

	flan_sub(flan1, flan2);

	printf("flan: %s\n", flan_as_str(flan1));
}

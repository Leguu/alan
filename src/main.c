#include "flan.h"
#include <stdio.h>

int main() {
	struct flan *flan1 = flan_init_int(5, 12);
	struct flan *flan2 = flan_init_int(5, 12);

	printf("ORIGINAL: FLAN1: %s, FLAN2: %s\n", flan_as_str(flan1),
	       flan_as_str(flan2));

	flan_mul(&flan1, flan2);

	printf("1: FLAN1: %s, FLAN2: %s\n", flan_as_str(flan1),
	       flan_as_str(flan2));
}

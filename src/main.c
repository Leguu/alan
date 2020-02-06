#include "flan.h"
#include <stdio.h>

int main() {
	Flan *flan1 = flan_init_str(5, "2");
	Flan *flan2 = flan_init_str(5, "40200");

	printf("%s has %d digits\n", flan_as_str(flan2), flan_digits(flan2));
	int i = 1;

	printf("%d: SELF: %s, OTHER: %s\n", i++, flan_as_str(flan1),
	       flan_as_str(flan2));

	flan_mul(&flan1, flan2);

	printf("%d: SELF: %s, OTHER: %s\n", i++, flan_as_str(flan1),
	       flan_as_str(flan2));
}

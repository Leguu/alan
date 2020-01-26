#include "flan.h"
#include <stdio.h>

int main() {
  struct flan *flan1 = flan_init_set(4, "20");
  struct flan *flan2 = flan_init_set(4, "21");

  printf("4: FLAN1: %s, FLAN2: %s\n", flan_as_str(flan1), flan_as_str(flan2));

  flan_sub(flan1, flan2);

  printf("5: FLAN1: %s, FLAN2: %s\n", flan_as_str(flan1), flan_as_str(flan2));

  flan_set(flan2, "500");
  flan_add(flan1, flan2);

  printf("6: FLAN1: %s, FLAN2: %s\n", flan_as_str(flan1), flan_as_str(flan2));

  flan_set(flan2, "100");
  flan_mod(flan1, flan2);

  printf("NEG: %d, DIGITS: %d%d%d%d, LEN: %d\n", flan1->neg, flan1->digits[3],
         flan1->digits[2], flan1->digits[1], flan1->digits[0], flan1->len);

  printf("NEG: %d, DIGITS: %d%d%d%d, LEN: %d\n", flan2->neg, flan2->digits[3],
         flan2->digits[2], flan2->digits[1], flan2->digits[0], flan2->len);

  printf("FLAN1: %s\n", flan_as_str(flan1));

  printf("7: FLAN1: %s, FLAN2: %s\n", flan_as_str(flan1), flan_as_str(flan2));
}

# ALAN: Arbitrary Length Arithmetic / Numbers

This is a basic single-header library for arbitrary length arithmetic. FLAN is Fixed Length Arithmetic / Numbers, which means there is a set number of digits. It will not adjust automatically with different number sizes. I intended these to be used for Euler's problems, so do not expect performance or effective use of memory.

This is the usage for FLAN:

```c
Flan *flan1 = flan_init_str(5, "2");
Flan *flan2 = flan_init_str(5, "200");

printf("SELF: %s, OTHER: %s\n", flan_as_str(flan1),
        flan_as_str(flan2));
// SELF: 2, OTHER: 200

flan_mul(&flan1, flan2);

printf("SELF: %s, OTHER: %s\n", flan_as_str(flan1),
        flan_as_str(flan2));
// SELF: 400, OTHER: 200

// We own flan1 and flan2, so we must release them when we're done.
flan_free(flan1);
flan_free(flan2);
```

Currently, FLAN has undefined behaviour if a number becomes larger than the number of digits. There are no checks, so make sure that there is enough space for what you need to do.

I am planning to create VLAN: Variable Length Arithmetic / Numbers, which will allow automatic resizing of the number.

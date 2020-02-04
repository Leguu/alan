#pragma once

//
// Include
//

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//
// Declarations
//

typedef char digit;

/// FIXED LENGTH ARITHMETIC / NUMBER
typedef struct {
	unsigned len;
	bool neg;
	digit *digits;
} Flan;

// Fundamental modifications

Flan *flan_init(int digits);
Flan *flan_init_str(int digits, char *target);
Flan *flan_init_int(int digits, long target);

bool flan_set_str(Flan *self, char *target);
void flan_set_int(Flan *self, long target);

// Operations

void flan_add_digits(Flan *self, const Flan *other);
void flan_add(Flan **self, const Flan *other);

void flan_sub_digits(Flan *self, const Flan *other);
void flan_sub(Flan **self, const Flan *other);

void flan_mod(Flan **self, const Flan *other);
void flan_div(Flan **self, const Flan *other);
void flan_mul(Flan **self, const Flan *other);

// Utility

int flan_cmp(const Flan *self, const Flan *other);
int flan_digits(const Flan *self);
char *flan_as_str(const Flan *self);

void flan_free(Flan *self);
Flan *flan_cpy(const Flan *self);

//
// Implementation
//

Flan *flan_init(int len) {
	Flan *self = malloc(sizeof(Flan));
	self->len = len;
	self->neg = false;
	self->digits = calloc(len, sizeof(digit));
	return self;
}

Flan *flan_cpy(const Flan *self) {
	Flan *result = flan_init(self->len);
	memcpy(result->digits, self->digits, self->len * sizeof(digit));
	result->neg = self->neg;
	return result;
}

void flan_free(Flan *self) {
	free(self->digits);
	free(self);
}

bool flan_set_str(Flan *self, char *str) {
	int len = strlen(str) - 1;

	// TODO: Check this? + 1 or not?
	if (len + 1 > self->len)
		return false;

	if (str[0] == '-')
		self->neg = true;
	else
		self->neg = false;

	bool recording = false;

	for (int i = 0; len >= self->neg; i++, len--) {
		if (str[len] != '0')
			recording = true;

		if (str[len] < '0' || '9' < str[len])
			return false;

		self->digits[i] = str[len] - '0';
	}
}

void flan_set_int(Flan *self, long target) {
	self->neg = target < 0;

	for (int i = 0; i < self->len; i++) {
		self->digits[i] = target % 10;
		target /= 10;
	}
}

Flan *flan_init_str(int len, char *str) {
	Flan *self = flan_init(len);
	flan_set_str(self, str);
	return self;
}

Flan *flan_init_int(int len, long target) {
	Flan *self = flan_init(len);
	flan_set_int(self, target);
	return self;
}

// Returns 0 if they're the same
// Returns -1 if other > self
// Returns 1 if self > other
int flan_cmp(const Flan *self, const Flan *other) {
	const int self_digits = flan_digits(self);
	const int other_digits = flan_digits(other);

	if (self_digits > other_digits)
		return 1;
	else if (self_digits < other_digits)
		return -1;

	for (int i = self_digits - 1; i >= 0; i--)
		if (self->digits[i] > other->digits[i])
			return 1;
		else if (self->digits[i] < other->digits[i])
			return -1;

	return 0;
}

void flan_add_digits(Flan *self, const Flan *other) {
	// First add all the digits together
	for (int i = 0; i < self->len; i++) {
		self->digits[i] += other->digits[i];
	}

	// Then carry over whatever is needed.
	for (int i = 0; i < self->len - 1; i++) {
		if (self->digits[i] > 9) {
			self->digits[i + 1] += self->digits[i] / 10;
			self->digits[i] = self->digits[i] % 10;
		}
	}
}

void flan_add(Flan **self, const Flan *other) {
	if (other->neg && (*self)->neg) {
		flan_add_digits(*self, other);
	} else if ((*self)->neg) {
		Flan *temp = flan_cpy(*self);
		temp->neg = false;

		flan_free(*self);
		*self = flan_cpy(other);

		flan_sub(self, temp);

		flan_free(temp);
	} else {
		flan_add_digits(*self, other);
	}
}

void flan_sub_digits(Flan *self, const Flan *other) {
	for (int i = self->len; i > 0; i--) {
		if (self->digits[i - 1] < other->digits[i - 1]) {
			self->digits[i] -= 1;
			self->digits[i - 1] += 10;
		}
		self->digits[i - 1] -= other->digits[i - 1];
	}
}

void flan_sub(Flan **self, const Flan *other) {
	if ((*self)->neg && other->neg) {
		flan_sub_digits(*self, other);
	} else if (other->neg || (*self)->neg) {
		flan_add_digits(*self, other);
	} else {
		// If both self and other are positive...
		// and if self is less than other
		if (flan_cmp(*self, other) < 0) {
			Flan *temp = flan_cpy(*self);

			flan_free(*self);
			*self = flan_cpy(other);
			(*self)->neg = true;

			flan_sub_digits(*self, temp);

			flan_free(temp);
		} else {
			flan_sub_digits(*self, other);
		}
	}
}

void flan_mod(Flan **self, const Flan *other) {
	do {
		flan_sub(self, other);
	} while (!(*self)->neg);
	flan_add(self, other);
}

void flan_div(Flan **self, const Flan *other) {
	int i = -1;
	while (!(*self)->neg) {
		i += 1;
		flan_sub(self, other);
	}
	flan_set_int(*self, i);
}

void flan_mul(Flan **self, const Flan *other) {
	Flan *original = flan_cpy(*self);
	Flan *i = flan_init_int((*self)->len, 1);
	Flan *one = flan_init_int(1, 1);
	for (; flan_cmp(i, other) != 0; flan_add(&i, one)) {
		flan_add(self, original);
	}
	flan_free(original);
	flan_free(one);
	flan_free(i);
}

int flan_digits(const Flan *self) {
	int i = 0;

	bool recording = true;
	for (; i < self->len; i++)
		if (self->digits[i] != 0)
			recording = false;
		else if (!recording)
			break;

	return (i == self->len ? 0 : i + 1);
}

char *flan_as_str(const Flan *self) {
	char *result = calloc(flan_digits(self) + 1, sizeof(char));
	char *curchar = result;
	bool recording = false;

	if (self->neg)
		*curchar++ = '-';

	for (int i = self->len; i >= 0; i--) {
		if (!recording && self->digits[i] == 0)
			continue;

		if (self->digits[i] != 0)
			recording = true;

		*curchar++ = '0' + self->digits[i];
	}

	return result;
}

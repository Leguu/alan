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

struct flan {
	unsigned len;
	bool neg;
	digit *digits;
};

struct flan *flan_init(int);
struct flan *flan_init_set(int, char *);
struct flan *flan_init_with(int, digit *);
bool flan_set(struct flan *, char *);
void flan_free(struct flan *);
struct flan *flan_cpy(const struct flan *);

void flan_add_digits(struct flan *, const struct flan *);
void flan_add(struct flan *, const struct flan *);
void flan_sub_digits(struct flan *, const struct flan *);
void flan_sub(struct flan *, const struct flan *);
void flan_mod(struct flan *, const struct flan *);

int flan_cmp(const struct flan *, const struct flan *);
int flan_num_digits(const struct flan *);
char *flan_as_str(const struct flan *);

//
// Implementation
//

struct flan *flan_init(int len) {
	struct flan *self = malloc(sizeof(struct flan));
	self->len = len;
	self->neg = false;
	self->digits = calloc(len, sizeof(digit));
	return self;
}

struct flan *flan_cpy(const struct flan *self) {
	struct flan *result = flan_init(self->len);
	memcpy(result->digits, self->digits, self->len * sizeof(digit));
	result->neg = self->neg;
	return result;
}

void flan_free(struct flan *self) {
	free(self->digits);
	free(self);
}

bool flan_set(struct flan *self, char *str) {
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

struct flan *flan_init_set(int len, char *str) {
	struct flan *self = flan_init(len);
	flan_set(self, str);
	return self;
}

// Returns 0 if they're the same
// Returns -1 if other > self
// Returns 1 if self > other
int flan_cmp(const struct flan *self, const struct flan *other) {
	const int self_digits = flan_num_digits(self);
	const int other_digits = flan_num_digits(other);

	if (self_digits > other_digits)
		return 1;
	if (other_digits > self_digits)
		return -1;

	for (int i = self_digits; i >= 0; i--) {
		if (self->digits[i] > other->digits[i])
			return 1;
		if (other->digits[i] > self->digits[i])
			return -1;
	}
	return 0;
}

void flan_add_digits(struct flan *self, const struct flan *other) {
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

void flan_add(struct flan *self, const struct flan *other) {
	if (other->neg && self->neg) {
		flan_add_digits(self, other);
	} else if (self->neg) {
		struct flan *temp = flan_cpy(self);
		temp->neg = false;

		flan_free(self);
		self = flan_cpy(other);

		flan_sub(self, temp);

		flan_free(temp);
	} else {
		flan_add_digits(self, other);
	}
}

void flan_sub_digits(struct flan *self, const struct flan *other) {
	for (int i = 0; i < self->len - 1; i++) {
		if (self->digits[i] < other->digits[i]) {
			for (int j = i + 1; j < self->len; j++) {
				if (j == i)
					break;
				if (self->digits[j] > 0) {
					self->digits[j] -= 1;
					self->digits[j - 1] += 10;
					j -= 2;
				}
			}
		}
		self->digits[i] -= other->digits[i];
	}
}

void flan_sub(struct flan *self, const struct flan *other) {
	if (self->neg && other->neg) {
		flan_sub_digits(self, other);
	} else if (other->neg || self->neg) {
		flan_add_digits(self, other);
	} else {
		// If both self and other are positive...
		// and if self is less than other
		if (flan_cmp(self, other) < 0) {
			struct flan *temp = flan_cpy(self);

			flan_free(self);
			self = flan_cpy(other);
			self->neg = true;

			flan_sub_digits(self, temp);

			flan_free(temp);
		} else {
			flan_sub_digits(self, other);
		}
	}
}

void flan_mod(struct flan *self, const struct flan *other) {
	do {
		flan_sub(self, other);
	} while (!self->neg);
	flan_add(self, other);
}

int flan_num_digits(const struct flan *self) {
	int i = 0;
	bool recording = true;
	for (; i < self->len; i++) {
		if (self->digits[i] != 0)
			recording = false;

		if (self->digits[i] == 0 && !recording)
			break;
	}
	return i + 1;
}

char *flan_as_str(const struct flan *self) {
	char *result = calloc(flan_num_digits(self) + 1, sizeof(char));
	char *curchar = result;
	bool recording = false;

	if (self->neg) {
		result[0] = '-';
		curchar += 1;
	}

	for (int i = self->len; i >= 0; i--) {
		if (!recording && self->digits[i] == 0)
			continue;

		if (self->digits[i] != 0)
			recording = true;

		*curchar++ = '0' + self->digits[i];
	}

	return result;
}

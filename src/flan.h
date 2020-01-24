#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef char digit;

struct flan {
	unsigned len;
	bool neg;
	digit *digits;
};

struct flan *flan_init(int);
void flan_free(struct flan *);
bool flan_set(struct flan *, char *);
struct flan *flan_init_set(int, char *);
int flan_cmp(const struct flan *, const struct flan *);
int flan_num_digits(const struct flan *);
char *flan_as_str(const struct flan *);

struct flan *flan_init(int len) {
	struct flan *self = malloc(sizeof(struct flan));
	self->len = len;
	self->neg = false;
	self->digits = calloc(len, sizeof(digit));
	return self;
}

void flan_free(struct flan *self) {
	free(self->digits);
	free(self);
}

bool flan_set(struct flan *self, char *str) {
	int len = strlen(str) - 1;

	if (len > self->len)
		return false;

	if (str[0] == '-')
		self->neg = true;

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

int flan_cmp(const struct flan *self, const struct flan *other) {
	const int self_digits = flan_num_digits(self);
	const int other_digits = flan_num_digits(other);

	if (self_digits > other_digits) return 1;
	if (other_digits > self_digits) return -1;

	for (int i = self_digits; i >= 0; i--) {
		if (self->digits[i] > other->digits[i]) return 1;
		if (other->digits[i] > self->digits[i]) return -1;
	}
	return 0;
}

void flan_add(struct flan *self, struct flan *other) {
	if (other->neg)
		; // flan_sub(self, other but negative);

	if (self->neg)
		; //Idk what to do here...

	// If both self and other are positive...

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

void flan_sub(struct flan *self, struct flan *other) {
	for (int i = 0; i < self->len - 1; i++) {
		if (self->digits[i] < other->digits[i]) {
			for (int j = i + 1; i < self->len; j++) {
				if (j == i)
					break;
				if (self->digits[j] > 0) {
					self->digits[j] -= 1;
					self->digits[j - 1] += 10;
					j -= 1;
				}
			}
		}
		self->digits[i] -= other->digits[i];
	}
}

int flan_num_digits(const struct flan *self) {
	int i = 0;
	bool recording = true;
	for (; ; i++) {
		if (self->digits[i] != 0) recording = false;

		if (self->digits[i] == 0
			&& !recording) return i;
	}
	return i;
}

char *flan_as_str(const struct flan *self) {
	char *result = calloc(flan_num_digits(self), sizeof(char));
	char *curchar = result;
	bool recording = false;

	if (self->neg) {
		result[0] = '-';
		curchar += 1;
	}

	for (int i = self->len; i >= 0; i--) {
		if (!recording && self->digits[i] == 0) continue;
		
		if (self->digits[i] != 0) recording = true;

		*curchar++ = '0' + self->digits[i];
	}

	return result;
}

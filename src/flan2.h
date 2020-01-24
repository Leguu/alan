#ifndef FLAN
#define FLAN

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef char digit;

struct flan {
	int length;
	digit *digits;
};

struct flan *flan_init(int length);
struct flan *flan_set_init(int length, char *value);

bool flan_set(struct flan *self, char *value);
void flan_add(struct flan *self, struct flan *other);
void flan_sub(struct flan *self, struct flan *other);
void flan_free(struct flan *self);
void flan_clear(struct flan *self);
void flan_printf(struct flan *self);
void flan_as_str(struct flan *self, char *str, int len);

#endif // FLAN

struct flan *flan_init(int length) {
	struct flan *result = malloc(sizeof(struct flan));
	result->length = length;
	result->digits = malloc(sizeof(digit) * length);
	flan_clear(result);
	return result;
}

bool flan_set(struct flan *self, char *value) {
	flan_clear(self);
	char *end_ptr = value + (sizeof(char) * strlen(value)) - sizeof(char);

	if(strlen(value) > self->length)
		return false;

	for (int i = 0; end_ptr >= value; i += 1, end_ptr -= sizeof(char)) {
		if (!isdigit(*end_ptr))
			return false;
		self->digits[i] = *end_ptr - '0';
	}

	return true;
}

struct flan *flan_set_init(int length, char *value) {
	struct flan *result = flan_init(length);
	if (flan_set(result, value))
		return result;
	else
		return NULL;
}

void flan_add(struct flan *self, struct flan *other) {
	for (int i = 0; i < self->length; i++) {
		self->digits[i] += other->digits[i];
	}

	for (int i = 0; i < self->length - 1; i++) {
		if (self->digits[i] > 9) {
			self->digits[i + 1] += self->digits[i] / 10;
			self->digits[i] = self->digits[i] % 10;
		}
	}
}

void flan_sub(struct flan *self, struct flan *other) {
	


}

void flan_free(struct flan *self) {
	free(self->digits);
	free(self);
}

void flan_clear(struct flan *self) {
	memset(self->digits, 0, sizeof(digit) * self->length);
}

void flan_print(struct flan *self) {
	char *str = malloc(sizeof(digit) * self->length);
	flan_as_str(self, str, self->length);
	printf("%s\n", str);
	free(str);
}

void flan_as_str(struct flan *self, char *str, int len) {
	if (self->length > len)
		return;

	bool recording = false;

	for (int i = self->length - 1; i >= 0; i--) {
		char cur_digit = self->digits[i];

		if (cur_digit == 0 &&
			!recording)
			continue;

		if (cur_digit != 0)
			recording = true;

		*str = cur_digit + '0';
		str++;
	}
}

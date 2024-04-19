#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdio.h>

#define DEFAULT_RESIZE_VALUE 10

typedef struct {
    double* data;
    size_t capacity;
    size_t offset;
} NumericVector;

typedef struct {
    char **data;
    size_t capacity;
    size_t offset;
    size_t *item_sizes;
} StringVector;

void set_debug(bool value);
bool numeric_vector_init(NumericVector *vector, size_t initial_size);
bool numeric_vector_add(NumericVector *vector, double value);
bool numeric_vector_reserve(NumericVector *vector, size_t spaces); /* With spaces I mean, e.g., 1 space = 4 bytes */
bool numeric_vector_shrink_to_fit(NumericVector *vector);
void numeric_vector_free(NumericVector *vector);
double numeric_vector_get_first(const NumericVector *vector);
double numeric_vector_get_at(const NumericVector *vector, size_t position);
double numeric_vector_get_last(const NumericVector *vector);
void numeric_vector_print(const NumericVector *vector);

bool string_vector_init(StringVector *vector, size_t initial_size);
void string_vector_free(StringVector *vector);
bool string_vector_add(StringVector *vector, const char *value);
bool string_vector_add_array(StringVector *vector, const char *array[], size_t n);
bool string_vector_reserve(StringVector *vector, size_t spaces); /* With spaces I mean, e.g., 1 space = 1 byte. */
bool string_vector_shrink_to_fit(StringVector *vector);
const char *string_vector_get_first(const StringVector *vector);
const char *string_vector_get_at(const StringVector *vector, size_t position);
const char *string_vector_get_last(const StringVector *vector);
size_t string_vector_strlen(const StringVector *vector, size_t position);
void string_vector_print(const StringVector *vector);

#endif // VECTOR_H

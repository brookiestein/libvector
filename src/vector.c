#include "vector.h"
#include "logger.h"

#include <stdlib.h>

bool debug = false;

void set_debug(bool value)
{
    debug = value;
}

bool numeric_vector_init(NumericVector *vector, size_t initial_size)
{
    logger(INFO, debug, __func__, __LINE__, "Initializing vector: %p to hold %li numbers...", vector, initial_size);

    vector->data = (double *) malloc(initial_size * sizeof(double));
    if (vector->data == NULL) {
        logger(ERROR, true, __func__, __LINE__, "Impossible to allocate %li bytes of memory for vector: %p.", initial_size, vector);
        return false;
    }

    logger(INFO, debug, __func__, __LINE__, "NumericVector: %p initialized with %li spaces.", vector, initial_size);

    vector->size = initial_size;
    vector->offset = 0;
    return true;
}

bool numeric_vector_add(NumericVector *vector, double value)
{
    logger(INFO, debug, __func__, __LINE__, "Adding value: %.2f to vector: %p...", value, vector);

    if (!vector->data) {
        logger(WARN, debug, __func__, __LINE__, "NumericVector isn't properly initialized. Initializing it with the default value: %i.",
                DEFAULT_RESIZE_VALUE);

        if (!numeric_vector_init(vector, DEFAULT_RESIZE_VALUE)) {
            logger(ERROR, true, __func__, __LINE__, "Can't continue.");
            return false;
        }
    }

    if (vector->offset + 1 > vector->size) {
        logger(INFO, debug, __func__, __LINE__, "Adding new value causes vector to be resized. Resizing with default resize value: %i",
                DEFAULT_RESIZE_VALUE);

        if (!numeric_vector_resize(vector, DEFAULT_RESIZE_VALUE)) {
            logger(ERROR, true, __func__, __LINE__, "Impossible to add value: %.2f.", value);
            return false;
        }
    }

    vector->data[vector->offset] = value;
    ++vector->offset;
    logger(INFO, debug, __func__, __LINE__, "Value: %.2f added to NumericVector: %p.", value, vector);
    return true;
}

bool numeric_vector_resize(NumericVector *vector, size_t spaces)
{
    size_t old_size = vector->size;
    size_t new_size = old_size + spaces;
    double tmp[old_size];

    logger(INFO, debug, __func__, __LINE__, "Resizing vector: %p...\nCurrent size: %li, new size: %li.", vector, old_size, new_size);
    logger(INFO, debug, __func__, __LINE__, "Saving vector contents to avoid data loss...");

    for (size_t i = 0; i < vector->size; ++i) {
        tmp[i] = vector->data[i];
    }

    logger(INFO, debug, __func__, __LINE__, "Vector contents saved. Freeing vector and reinitializing it with the requested size...");
    numeric_vector_free(vector);

    /* TODO: Implement a way to roll back and leave the original vector as it was received. */
    if (!numeric_vector_init(vector, new_size)) {
        logger(ERROR, true, __func__, __LINE__, "Impossible to resize vector. Exiting...");
        return false;
    }

    logger(INFO, debug, __func__, __LINE__, "Vector resized. New size is: %li. Restoring saved vector data...", vector->size);

    for (size_t i = 0; i < old_size; ++i) {
        numeric_vector_add(vector, tmp[i]); // This operation shouldn't fail, so I'm not checking whether it succeded.
    }

    logger(INFO, debug, __func__, __LINE__, "Vector contents restored successfully.");

    return true;
}

void numeric_vector_free(NumericVector *vector)
{
    if (!vector->data && vector->size == 0 && vector->offset == 0) {
        logger(ERROR, true, __func__, __LINE__, "No need to free vector.");
        return;
    }

    logger(INFO, debug, __func__, __LINE__, "Freeing vector: %p...", vector);

    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->offset = 0;

    logger(INFO, debug, __func__, __LINE__, "Vector: %p freed.", vector);
}

void numeric_vector_print(const NumericVector *vector)
{
    if (!vector->data) {
        logger(
                ERROR, true, __func__, __LINE__,
                "Vector isn't properly initialized. Please call int_vector_init() and int_vector_add() before using this function."
        );
        return;
    }

    for (size_t i = 0; i < vector->offset; ++i) {
        printf("%.2f%s", vector->data[i], (i < vector->offset - 1 ? ", " : "\n"));
    }

    printf("Vector size: %li\n", vector->size);
    printf("Vector values: %li\n", vector->offset);
}

bool string_vector_init(StringVector *vector, size_t initial_size)
{
    logger(INFO, debug, __func__, __LINE__, "Initializing StringVector with %li bytes.", initial_size);

    vector->data = (char **) malloc(initial_size * sizeof(char *));
    if (!vector->data) {
        logger(ERROR, true, __func__, __LINE__, "Impossible to initialize string vector with size: %li.", initial_size);
        return false;
    }

    vector->item_sizes = (size_t *) malloc(initial_size * sizeof(size_t));
    if (!vector->item_sizes) {
        logger(ERROR, true, __func__, __LINE__, "%s: Impossible to allocate memory for string vector.");
        string_vector_free(vector);
        return false;
    }

    vector->vector_size = initial_size;
    vector->offset = 0;

    for (size_t i = 0; i < vector->vector_size; ++i) {
        vector->data[i] = NULL;
    }

    logger(INFO, debug, __func__, __LINE__, "Vector: %p initialized with %li spaces.", vector, initial_size);

    return true;
}

void string_vector_free(StringVector *vector)
{
    logger(INFO, debug, __func__, __LINE__, "Freeing StringVector: %p...", vector);

    if (vector->data != NULL) {
        for (size_t i = 0; i < vector->vector_size; ++i) {
            if (vector->data[i] != NULL) {
                logger(INFO, debug, __func__, __LINE__, "Freeing StringVector item #%li: %p...", i, vector->data[i]);
                free(vector->data[i]);
                logger(INFO, debug, __func__, __LINE__, "StringVector item #%li: %p freed.", i, vector->data[i]);
                vector->data[i] = NULL;
            }
        }

        logger(INFO, debug, __func__, __LINE__, "Freeing StringVector data pointer: %p...", vector->data);
        free(vector->data);
        logger(INFO, debug, __func__, __LINE__, "StringVector data: %p freed...", vector->data);
        vector->data = NULL;
    }

    if (vector->item_sizes != NULL) {
        free(vector->item_sizes);
        vector->item_sizes = NULL;
    }

    vector->vector_size = 0;
    vector->offset = 0;

    logger(INFO, debug, __func__, __LINE__, "StringVector: %p freed.", vector);
}

static size_t string_vector_item_strlen(const char *item)
{
    logger(INFO, debug, __func__, __LINE__, "Calculating string size...");

    size_t size = 0;
    while (item[size] != '\0') {
        ++size;
    }

    logger(INFO, debug, __func__, __LINE__, "Deducted string size is %li.", size);
    return size;
}

/* Blindly trust on from having at least n items, and to having at least n bytes. */
static void string_vector_copy_item(const char *from, char *to, size_t n)
{
    logger(INFO, debug, __func__, __LINE__, "Copying string: %s into %p...", from, to);

    size_t i;
    for (i = 0; i < n; ++i) {
        to[i] = from[i];
    }
    to[i] = '\0';

    logger(INFO, debug, __func__, __LINE__, "%li bytes were copied to %p.", i, to);
}

bool string_vector_add(StringVector *vector, const char *value)
{
    logger(INFO, debug, __func__, __LINE__, "Adding value: %s to vector: %p...", value, vector);

    if (!vector->data) {
        logger(WARN, debug, __func__, __LINE__, "StringVector isn't properly initialized. Initializing it with the default size value: %i.",
                DEFAULT_RESIZE_VALUE);

        if (!string_vector_init(vector, DEFAULT_RESIZE_VALUE)) {
            logger(ERROR, true, __func__, __LINE__, "Impossible to initialize StringVector. Not continuing.");
            return false;
        }
    }

    if (vector->offset + 1 > vector->vector_size) {
        logger(
                INFO, debug, __func__, __LINE__,
                "Adding new value causes vector to be resized. Resizing with the default size value: %i",
                DEFAULT_RESIZE_VALUE
        );

        if (!string_vector_resize(vector, DEFAULT_RESIZE_VALUE)) {
            logger(ERROR, true, __func__, __LINE__, "StringVector couldn't be resized. Not continuing.");
            return false;
        }
    }

    size_t size = string_vector_item_strlen(value);
    vector->data[vector->offset] = (char *) malloc((size + 1) * sizeof(char));

    if (!vector->data[vector->offset]) {
        logger(ERROR, true, __func__, __LINE__, "Impossible to allocate memory for string value: %s. Not adding.", value);
        return false;
    }

    string_vector_copy_item(value, vector->data[vector->offset], size);
    vector->item_sizes[vector->offset] = size + 1;
    ++vector->offset;

    logger(INFO, debug, __func__, __LINE__, "Value: %s added to vector: %p.", vector->data[vector->offset - 1], vector);

    return true;
}

bool string_vector_add_array(StringVector *vector, const char *values[], size_t n)
{
    if (!vector->data) {
        logger(
                ERROR, true, __func__, __LINE__,
                "String Vector: %p isn't properly initialized. Please call string_vector_init() before using this function.",
                vector
        );
        return false;
    }

    if (vector->offset + n > vector->vector_size) {
        logger(INFO, debug, __func__, __LINE__, "Adding array causes vector to be resized. Resizing with %li more spaces...", n);

        if (!string_vector_resize(vector, n)) {
            logger(ERROR, true, __func__, __LINE__, "Impossible to resize vector. Not continuing.");
            return false;
        }
    }

    size_t i;
    for (i = 0; i < n; ++i) {
        if (!string_vector_add(vector, values[i])) {
            logger(
                    ERROR, true, __func__, __LINE__,
                    "Impossible to add value: %s to vector: %p. Not continuing.",
                    values[i], vector
            );
            break;
        }
    }

    logger(INFO, debug, __func__, __LINE__, "%li new values added to vector: %p.", i, vector);

    return i == n;
}

bool string_vector_resize(StringVector *vector, size_t spaces)
{
    if (!vector->data) {
        logger(WARN, true, __func__, __LINE__, "StringVector isn't properly initialized. Not resizing.");
        return false;
    }

    size_t old_size = vector->vector_size;
    size_t new_size = old_size + spaces;
    size_t item_sizes[old_size];
    size_t vector_initial_length = 0;
    char *tmp[old_size];

    logger(
            INFO, debug, __func__, __LINE__,
            "Resizing vector: %p... Current size: %li, new size: %li",
            vector, old_size, new_size
    );

    logger(INFO, debug, __func__, __LINE__, "Saving vector contents to avoid data loss...");

    /*
     * Back up and restore actual vector items.
     * If vector size is 10, for example, but its actual contents are just 8, i.e current offset
     * just back up those 8 instead of trying to access to memory address 9 when it's currently NULL until it's actually used.
     */
    size_t stop = new_size < old_size ? new_size : old_size;
    for (size_t i = 0; i < stop; ++i) {
        ++vector_initial_length;
        item_sizes[i] = vector->item_sizes[i];
        tmp[i] = (char *) malloc((item_sizes[i]) * sizeof(char)); /* item_sizes[i] already takes in account the \0 character. */

        if (tmp[i] == NULL) {
            logger(
                    ERROR, true, __func__, __LINE__,
                    "Error allocating memory to save vector value: %s. Not continuing.",
                    vector->data[i]
            );
            return false;
        }

        string_vector_copy_item(vector->data[i], tmp[i], item_sizes[i] - 1); /* item_sizes[i] equals to string_vector_strlen() + 1 */
    }

    logger(
            INFO, debug, __func__, __LINE__,
            "Vector contents saved. Freeing vector: %p and reinitializing it with new size: %li...",
            vector, new_size
    );

    string_vector_free(vector);

    /* TODO: Roll vector back to its initial state. */
    if (!string_vector_init(vector, new_size)) {
        logger(ERROR, true, __func__, __LINE__, "Error initializing vector: %p with new size: %li.", vector, new_size);
        return false;
    }

    logger(INFO, debug, __func__, __LINE__, "Restoring vector items...");

    size_t i;
    for (i = 0; i < stop; ++i) {
        vector->data[vector->offset] = (char *) malloc((item_sizes[i] + 1) * sizeof(char));
        if (!vector->data[vector->offset]) {
            logger(
                    ERROR, true, __func__, __LINE__,
                    "Impossible to restore vector item: %s. Memory couldn't be allocated.",
                    tmp[i]
            );
            break;
        }

        string_vector_copy_item(tmp[i], vector->data[vector->offset], item_sizes[i]);
        ++vector->offset;
    }

    bool result = i == vector_initial_length;
    logger(INFO, debug, __func__, __LINE__, "%li values were restored.", i);

    /* If making vector longer, set the remaining items to NULL. */
    if (vector->vector_size > vector->offset) {
        for (size_t i = vector->offset; i < vector->vector_size; ++i) {
            vector->data[i] = NULL;
        }
    }

    return result;
}

bool string_vector_shrink(StringVector *vector)
{
    if (!vector->data) {
        logger(ERROR, true, __func__, __LINE__, "StringVector isn't properly initialized. Please call:");
        logger(
                ERROR, true, __func__, __LINE__,
                "string_vector_init() and string_vector_add()/string_vector_add_array() before using this function."
        );
        return false;
    }

    if (vector->vector_size == vector->offset) {
        logger(WARN, debug, __func__, __LINE__, "No need to shrink vector: %p.", vector);
        return true;
    }

    size_t old_size = vector->vector_size;
    size_t new_size = vector->offset;
    logger(
            INFO, debug, __func__, __LINE__,
            "Shrinking vector: %p... Old size: %li, new size: %li.",
            vector, old_size, new_size
    );

    if (!string_vector_resize(vector, -(old_size - new_size))) {
        logger(ERROR, true, __func__, __LINE__, "Error: Cannot shrink vector: %p. Not continuing.", vector);
        return false;
    }

    logger(
            INFO, debug, __func__, __LINE__,
            "StringVector: %p shrinked successfully. New size is: %li.",
            vector, vector->vector_size
    );

    return true;
}

void string_vector_print(const StringVector *vector)
{
    if (!vector->data) {
        logger(
                ERROR, true, __func__, __LINE__,
                "StringVector isn't properly initialized."
        );

        logger(
                ERROR, true, __func__, __LINE__,
                "Please call string_vector_init() and string_vector_add() before using this function."
        );

        return;
    }

    for (size_t i = 0; i < vector->offset; ++i) {
        printf("%s%s",
                vector->data[i],
                (i < vector->offset - 1 ? ", " : "\n"));
    }

    printf("Vector size: %li\n", vector->vector_size);
    printf("Vector items: %li\n", vector->offset);
}

#include "vector.h"
#include "logger.h"

#include <errno.h>
#include <stdlib.h>

static bool debug = false;

void set_debug(bool value)
{
    debug = value;
}

const char *libvector_version(void)
{
    return "libvector v"LIBVECTOR_VERSION;
}

bool numeric_vector_init(NumericVector *vector, size_t initial_size)
{
    logger(INFO, debug, __func__, __LINE__, "Initializing vector: %p to hold %li numbers...", vector, initial_size);

    vector->data = (double *) malloc(initial_size * sizeof(double));
    if (vector->data == NULL) {
        logger(
                ERROR, true, __func__, __LINE__,
                "Impossible to allocate %li bytes of memory for vector: %p. Error code: %i",
                initial_size, vector, errno
        );

        return false;
    }

    logger(INFO, debug, __func__, __LINE__, "NumericVector: %p initialized with %li spaces.", vector, initial_size);

    vector->capacity = initial_size;
    vector->offset = 0;
    return true;
}

void numeric_vector_free(NumericVector *vector)
{
    if (!vector->data && vector->capacity == 0 && vector->offset == 0) {
        logger(ERROR, true, __func__, __LINE__, "No need to free vector.");
        return;
    }

    logger(INFO, debug, __func__, __LINE__, "Freeing vector: %p...", vector);

    free(vector->data);
    vector->data = NULL;
    vector->capacity = 0;
    vector->offset = 0;

    logger(INFO, debug, __func__, __LINE__, "Vector: %p freed.", vector);
}

static bool numeric_vector_is_valid(const NumericVector *vector, const char *func, int line, bool show_suggestions)
{
    char suggestion[] = "Please call numeric_vector_init() before using this function.";

    if (vector->data == NULL) {
        logger(
                ERROR, true, func, line,
                "NumericVector: %p isn't properly initialized.%s",
                vector,
                (show_suggestions ? suggestion : "")
        );

        return false;
    }

    return true;
}

bool numeric_vector_add(NumericVector *vector, double value)
{
    logger(INFO, debug, __func__, __LINE__, "Adding value: %.2f to vector: %p...", value, vector);

    if (!numeric_vector_is_valid(vector, __func__, __LINE__, false)) {
        logger(WARN, debug, __func__, __LINE__, "Initializing it with the default value: %i.",
                DEFAULT_RESIZE_VALUE);

        if (!numeric_vector_init(vector, DEFAULT_RESIZE_VALUE)) {
            logger(ERROR, true, __func__, __LINE__, "Can't continue.");
            return false;
        }
    }

    if (vector->offset + 1 > vector->capacity) {
        logger(INFO, debug, __func__, __LINE__, "Adding new value causes vector to be resized. Resizing with default resize value: %i",
                DEFAULT_RESIZE_VALUE);

        if (!numeric_vector_reserve(vector, DEFAULT_RESIZE_VALUE)) {
            logger(ERROR, true, __func__, __LINE__, "Impossible to add value: %.2f.", value);
            return false;
        }
    }

    vector->data[vector->offset] = value;
    ++vector->offset;
    logger(INFO, debug, __func__, __LINE__, "Value: %.2f added to NumericVector: %p.", value, vector);
    return true;
}

bool numeric_vector_add_array(NumericVector *vector, double *numbers, size_t size)
{
    if (!numeric_vector_is_valid(vector, __func__, __LINE__, true)) {
        return false;
    }

    if (vector->offset + size > vector->capacity) {
        logger(
                INFO, debug, __func__, __LINE__,
                "Adding array values causes vector to be resized. Resizing to hold %li more numbers.",
                size);

        if (!numeric_vector_reserve(vector, size)) {
            logger(
                    ERROR, true, __func__, __LINE__,
                    "Impossible to reserve %li more spaces. Not continuing.",
                    size
            );

            return false;
        }
    }

    size_t i;
    for (i = 0; i < size; ++i) {
        vector->data[vector->offset] = numbers[i];
        ++vector->offset;
    }

    logger(INFO, debug, __func__, __LINE__, "%li new values were added to NumericVector: %p.", i, vector);
    return i == size;
}

bool numeric_vector_reserve(NumericVector *vector, size_t spaces)
{
    if (!numeric_vector_is_valid(vector, __func__, __LINE__, true)) {
        return false;
    }

    size_t new_capacity = vector->capacity + spaces;
    logger(
            INFO, debug, __func__, __LINE__,
            "Reserving %li more spaces to NumericVector: %p... Current capacity: %li, New capacity: %li.",
            spaces, vector, vector->capacity, new_capacity
    );

    NumericVector new_vector;
    if (!numeric_vector_init(&new_vector, new_capacity)) {
        logger(
                ERROR, true, __func__, __LINE__,
                "Impossible to reserve %li more spaces. Leaving original NumericVector as it was received.",
                spaces
        );

        return false;
    }

    for (size_t i = 0; i < vector->capacity; ++i) {
        new_vector.data[i] = vector->data[i];
        ++new_vector.offset;
    }

    numeric_vector_free(vector);
    vector->data = new_vector.data;
    vector->capacity = new_vector.capacity;
    vector->offset = new_vector.offset;

    logger(
            INFO, debug, __func__, __LINE__,
            "%li more spaces reserved to NumericVector: %p.",
            spaces, vector
    );

    return true;
}

bool numeric_vector_shrink_to_fit(NumericVector *vector)
{
    if (!numeric_vector_is_valid(vector, __func__, __LINE__, true)) {
        return false;
    }

    if (vector->capacity == vector->offset) {
        logger(WARN, debug, __func__, __LINE__, "No need to shrink NumericVector: %p.", vector);
        return true;
    }

    size_t new_capacity = vector->offset;

    logger(
            INFO, debug, __func__, __LINE__,
            "Shrinking NumericVector: %p... Current capacity: %li, New capacity: %li.",
            vector, vector->capacity, new_capacity
    );

    NumericVector new_vector;
    if (!numeric_vector_init(&new_vector, new_capacity)) {
        logger(
                ERROR, true, __func__, __LINE__,
                "Impossible to shrink NumericVector: %p. Couldn't make backup of existing data.",
                vector
        );

        return false;
    }

    for (size_t i = 0; i < new_capacity; ++i) {
        new_vector.data[i] = vector->data[i];
        ++new_vector.offset;
    }

    numeric_vector_free(vector);
    vector->data = new_vector.data;
    vector->capacity = new_vector.capacity;
    vector->offset = new_vector.offset;

    logger(
            INFO, debug, __func__, __LINE__, "NumericVector: %p shrinked. New capacity is %li.",
            vector, vector->capacity
    );

    return true;
}

bool numeric_vector_clear(NumericVector *vector)
{
    if (!numeric_vector_is_valid(vector, __func__, __LINE__, true)) {
        return false;
    }

    size_t capacity = vector->capacity;
    numeric_vector_free(vector);

    if (!numeric_vector_init(vector, capacity)) {
        logger(
                WARN, true, __func__, __LINE__,
                "NumericVector: %p was cleared, but it wasn't possible to initialize. It shouldn't be used until properly initialized.",
                vector
        );
    }

    return true;
}

/* Take out and return last item on vector or -1 on failure. */
double numeric_vector_pop(NumericVector *vector)
{
    if (!numeric_vector_is_valid(vector, __func__, __LINE__, true) || vector->offset == 0) {
        return -1;
    }

    size_t index = vector->offset - 1;
    double value = vector->data[index];
    vector->data[index] = -1;
    --vector->offset;
    return value;
}

/* numeric_vector_get_* set of functions return -1 on failure, e.g., position >= vector bounds. */
double numeric_vector_get_first(const NumericVector *vector)
{
    return numeric_vector_get_at(vector, 0);
}

double numeric_vector_get_at(const NumericVector *vector, size_t position)
{
    if (!numeric_vector_is_valid(vector, __func__, __LINE__, true)) {
        return -1;
    }

    if (position >= vector->offset) {
        logger(
                ERROR, true, __func__, __LINE__,
                "NumericVector: %p doesn't have any value on position: %li.",
                vector, position);

        return -1;
    }

    return vector->data[position];
}

double numeric_vector_get_last(const NumericVector *vector)
{
    return numeric_vector_get_at(vector, vector->offset - 1);
}

void numeric_vector_print(const NumericVector *vector)
{
    if (!numeric_vector_is_valid(vector, __func__, __LINE__, true)) {
        return;
    }

    for (size_t i = 0; i < vector->offset; ++i) {
        printf("%.2f%s", vector->data[i], (i < vector->offset - 1 ? ", " : "\n"));
    }

    printf("Vector capacity: %li\n", vector->capacity);
    printf("Vector items: %li\n", vector->offset);
}

bool string_vector_init(StringVector *vector, size_t initial_size)
{
    logger(INFO, debug, __func__, __LINE__, "Initializing StringVector with capacity to hold %li strings.", initial_size);

    vector->data = (char **) malloc(initial_size * sizeof(char *));
    if (!vector->data) {
        logger(
                ERROR, true, __func__, __LINE__,
                "Impossible to initialize string vector with size: %li. Error code: %i",
                initial_size, errno
        );

        return false;
    }

    vector->item_sizes = (size_t *) malloc(initial_size * sizeof(size_t));
    if (vector->item_sizes == NULL) {
        logger(
                ERROR, true, __func__, __LINE__,
                "Impossible to allocate memory for StringVector. Error code: %i",
                errno
        );

        string_vector_free(vector);
        return false;
    }

    vector->capacity = initial_size;
    vector->offset = 0;

    for (size_t i = 0; i < vector->capacity; ++i) {
        vector->data[i] = NULL;
    }

    logger(INFO, debug, __func__, __LINE__, "Vector: %p initialized with %li spaces.", vector, initial_size);

    return true;
}

static bool string_vector_is_valid(const StringVector *vector, const char *func, int line, bool show_suggestions)
{
    char suggestion[] = "Please call string_vector_init() before using this function.";

    if (vector->data == NULL) {
        logger(
                ERROR, true, func, line,
                "StringVector: %p isn't properly initialized.%s",
                vector,
                (show_suggestions ? suggestion : "")
        );

        return false;
    }

    return true;
}

void string_vector_free(StringVector *vector)
{
    logger(INFO, debug, __func__, __LINE__, "Freeing StringVector: %p...", vector);

    if (vector->data != NULL) {
        for (size_t i = 0; i < vector->capacity; ++i) {
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

    vector->capacity = 0;
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

    if (!string_vector_is_valid(vector, __func__, __LINE__, false)) {
        logger(WARN, debug, __func__, __LINE__, "Initializing it with the default size value: %i.",
                DEFAULT_RESIZE_VALUE);

        if (!string_vector_init(vector, DEFAULT_RESIZE_VALUE)) {
            logger(ERROR, true, __func__, __LINE__, "Impossible to initialize StringVector. Not continuing.");
            return false;
        }
    }

    if (vector->offset + 1 > vector->capacity) {
        logger(
                INFO, debug, __func__, __LINE__,
                "Adding new value causes vector to be resized. Resizing with the default size value: %i",
                DEFAULT_RESIZE_VALUE
        );

        if (!string_vector_reserve(vector, DEFAULT_RESIZE_VALUE)) {
            logger(ERROR, true, __func__, __LINE__, "StringVector couldn't be resized. Not continuing.");
            return false;
        }
    }

    size_t size = string_vector_item_strlen(value);
    vector->data[vector->offset] = (char *) malloc((size + 1) * sizeof(char));

    if (vector->data[vector->offset] == NULL) {
        logger(
                ERROR, true, __func__, __LINE__,
                "Impossible to allocate memory for string value: %s. Error code: %i",
                value, errno
        );

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
    if (!string_vector_is_valid(vector, __func__, __LINE__, true)) {
        return false;
    }

    if (vector->offset + n > vector->capacity) {
        logger(INFO, debug, __func__, __LINE__, "Adding array causes vector to be resized. Resizing with %li more spaces...", n);

        if (!string_vector_reserve(vector, n)) {
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

bool string_vector_reserve(StringVector *vector, size_t spaces)
{
    if (!string_vector_is_valid(vector, __func__, __LINE__, true)) {
        return false;
    }

    size_t old_capacity = vector->capacity;
    size_t new_capacity = old_capacity + spaces;
    StringVector new_vector;

    logger(
            INFO, debug, __func__, __LINE__,
            "Reserving %li more spaces for vector: %p... Old capacity: %li, new capacity: %li",
            spaces, vector, old_capacity, new_capacity
    );

    if (!string_vector_init(&new_vector, new_capacity)) {
        logger(ERROR, true, __func__, __LINE__, "Impossible to reserve %li more spaces for vector: %p.", spaces, vector);
        return false;
    }

    logger(INFO, debug, __func__, __LINE__, "Saving old StringVector items into new StringVector...");

    for (size_t i = 0; i < vector->offset; ++i) {
        size_t size = vector->item_sizes[i];

        logger(INFO, debug, __func__, __LINE__, "Allocating memory for StringVector item: %s...", vector->data[i]);
        new_vector.data[i] = (char *) malloc(size * sizeof(char));

        if (!new_vector.data[i]) {
            logger(
                    INFO, true, __func__, __LINE__,
                    "Impossible to allocate memory for StringVector item: %s! Error code: %i. Leaving old StringVector as it was received.",
                    vector->data[i], errno
            );

            string_vector_free(&new_vector);
            return false;
        }

        logger(INFO, debug, __func__, __LINE__, "Memory allocated. Copying StringVector item: %s...", vector->data[i]);

        string_vector_copy_item(vector->data[i], new_vector.data[i], size - 1); // vector->item_sizes[i] has 1 more because of \0
        new_vector.item_sizes[i] = size;
        ++new_vector.offset;

        logger(INFO, debug, __func__, __LINE__, "StringVector item: %s copied.", vector->data[i]);
    }

    logger(
            INFO, debug, __func__, __LINE__,
            "Freeing old StringVector: %p, and pointing to new StringVector: %p",
            vector, &new_vector
    );

    string_vector_free(vector);
    vector->data = new_vector.data;
    vector->item_sizes = new_vector.item_sizes;
    vector->capacity = new_vector.capacity;
    vector->offset = new_vector.offset;
    return true;
}

bool string_vector_shrink_to_fit(StringVector *vector)
{
    if (!string_vector_is_valid(vector, __func__, __LINE__, true)) {
        return false;
    }

    if (vector->capacity == vector->offset) {
        logger(WARN, debug, __func__, __LINE__, "No need to shrink vector: %p.", vector);
        return true;
    }

    logger(
            INFO, debug, __func__, __LINE__,
            "Shrinking StringVector: %p... Current capacity: %li, New capacity: %li",
            vector, vector->capacity, vector->offset
    );

    size_t new_capacity = vector->offset;
    StringVector new_vector;
    if (!string_vector_init(&new_vector, new_capacity)) {
        logger(
                ERROR, true, __func__, __LINE__,
                "Impossible to shrink vector: %p. Leaving original vector as it was received."
        );
        return false;
    }

    for (size_t i = 0; i < new_capacity; ++i) {
        logger(INFO, debug, __func__, __LINE__, "Backing up StringVector item: %s...", vector->data[i]);
        size_t item_size = vector->item_sizes[i];
        new_vector.data[i] = (char *) malloc(item_size * sizeof(char)); /* item_sizes[i] already takes in account the \0 character. See string_vector_init() */

        if (new_vector.data[i] == NULL) {
            logger(
                    ERROR, true, __func__, __LINE__,
                    "Impossible to backup StringVector item: %s. Error code: %i. Leaving original vector as it was received.",
                    vector->data[i], errno
            );

            string_vector_free(&new_vector);
            return false;
        }

        new_vector.item_sizes[i] = item_size;
        string_vector_copy_item(vector->data[i], new_vector.data[i], item_size - 1); /* item_sizes[i] has 1 more space for the \0 character. */
        ++new_vector.offset;

        logger(INFO, debug, __func__, __LINE__, "StringVector item: %s backed up.", vector->data[i]);
    }

    logger(
            INFO, debug, __func__, __LINE__,
            "All items in StringVector: %p were backed up. Freeing original vector and making it point to the new one.",
            vector
    );

    string_vector_free(vector);
    vector->data = new_vector.data;
    vector->item_sizes = new_vector.item_sizes;
    vector->capacity = new_vector.capacity;
    vector->offset = new_vector.offset;

    logger(INFO, debug, __func__, __LINE__, "StringVector: %p shrinked. New capacity is: %li.", vector, vector->capacity);
    return true;
}

bool string_vector_clear(StringVector *vector)
{
    if (!string_vector_is_valid(vector, __func__, __LINE__, true)) {
        return false;
    }

    size_t capacity = vector->capacity;
    string_vector_free(vector);

    if (!string_vector_init(vector, capacity)) {
        logger(
                WARN, true, __func__, __LINE__,
                "StringVector: %p was cleared, but it wasn't possible to re-initialize. It shouldn't be used until properly initialized.",
                vector
        );
    }

    return true;
}

/* Returns the last heap-allocated string. Remember to free() it! */
char *string_vector_pop(StringVector *vector)
{
    if (!string_vector_is_valid(vector, __func__, __LINE__, true) || vector->offset == 0) {
        return NULL;
    }

    size_t index = vector->offset - 1;
    char *last = vector->data[index];
    vector->data[index] = NULL;
    --vector->offset;
    return last;
}

/* string_vector_get_* set of functions return NULL on failure, e.g., position >= vector bounds. */
const char *string_vector_get_first(const StringVector *vector)
{
    return string_vector_get_at(vector, 0);
}

const char *string_vector_get_at(const StringVector *vector, size_t position)
{
    if (!string_vector_is_valid(vector, __func__, __LINE__, true)) {
        return NULL;
    }

    if (position >= vector->offset) {
        logger(
                ERROR, true, __func__, __LINE__,
                "StringVector: %p doesn't have any value on position: %li.",
                vector, position
        );

        return NULL;
    }

    return vector->data[position];
}

const char *string_vector_get_last(const StringVector *vector)
{
    return string_vector_get_at(vector, vector->offset - 1);
}

size_t string_vector_strlen(const StringVector *vector, size_t position)
{
    if (!string_vector_is_valid(vector, __func__, __LINE__, true)) {
        return 0;
    }

    if (vector->offset == 0) {
        logger(
                WARN, debug, __func__, __LINE__,
                "StringVector has no string. Consider using string_vector_add{,_array}() before using this function."
        );

        return 0;
    }

    size_t length = vector->item_sizes[position];
    return length == 0 ? length : length - 1;
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

    printf("StringVector capacity: %li\n", vector->capacity);
    printf("StringVector items: %li\n", vector->offset);
}

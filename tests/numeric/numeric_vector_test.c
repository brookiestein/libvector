#include "vector.h"

int main(void)
{
    // set_debug(true);

    NumericVector numbers;
    printf("[TEST]: Initializing NumericVector to hold 3 numbers.\n");

    if (numeric_vector_init(&numbers, 3)) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        return 1;
    }

    printf("[TEST]: Add first value to NumericVector.\n");

    if (numeric_vector_add(&numbers, 1)) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        numeric_vector_free(&numbers);
        return 1;
    }

    printf("[TEST]: Add second value to NumericVector.\n");

    if (numeric_vector_add(&numbers, 2)) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        numeric_vector_free(&numbers);
        return 1;
    }

    printf("[TEST]: Add third value to NumericVector.\n");

    if (numeric_vector_add(&numbers, 3)) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        numeric_vector_free(&numbers);
        return 1;
    }

    printf("[TEST]: Printing vector.\n");
    numeric_vector_print(&numbers);

    printf("[TEST]: Add fourth value to NumericVector.\n");

    if (numeric_vector_add(&numbers, 4)) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        numeric_vector_free(&numbers);
        return 1;
    }

    printf("[TEST]: Printing vector.\n");
    numeric_vector_print(&numbers);

    printf("[TEST]: Shrinking vector to fit its actual contents.\n");

    if (numeric_vector_shrink_to_fit(&numbers)) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        numeric_vector_free(&numbers);
        return 1;
    }

    printf("[TEST]: Printing vector.\n");
    numeric_vector_print(&numbers);

    printf("[TEST]: Getting first item: %.f\n", numeric_vector_get_first(&numbers));
    printf("[TEST]: Getting second item: %.f\n", numeric_vector_get_at(&numbers, 1));
    printf("[TEST]: Getting fifth item: %.f?\n", numeric_vector_get_at(&numbers, 4));
    printf("[TEST]: Getting last item: %.f\n", numeric_vector_get_last(&numbers));

    printf("[TEST]: Pop-ing NumericVector.\n");
    double last = numeric_vector_pop(&numbers);

    if (last >= 0) {
        printf("[TEST]: Passed!\n");
        printf("Pop-ed item: %.f\n", last);
    } else {
        printf("[TEST]: Failed!\n");
    }

    printf("[TEST]: Printing vector.\n");
    numeric_vector_print(&numbers);

    printf("[TEST]: Add fourth value to NumericVector (again).\n");

    if (numeric_vector_add(&numbers, 42)) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        numeric_vector_free(&numbers);
        return 1;
    }

    printf("[TEST]: Printing vector.\n");
    numeric_vector_print(&numbers);

    printf("[TEST]: Copying uninitialized vector.\n");
    NumericVector copy;
    if (numeric_vector_copy(&numbers, &copy, true)) {
        printf("[TEST]: Passed!\n");
        printf("[TEST]: Printing vector.\n");
        numeric_vector_print(&copy);
        numeric_vector_free(&copy);
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
    }

    printf("[TEST]: Copying initialized vector, but forcing it to be resized.\n");
    NumericVector copy2;
    if (numeric_vector_init(&copy2, 3)) {
        printf("[TEST]: Filling vector.\n");
        numeric_vector_add(&copy2, 75);
        numeric_vector_add(&copy2, 10);
        numeric_vector_add(&copy2, 30);
        printf("[TEST]: Printing vector.\n");
        numeric_vector_print(&copy2);

        printf("[TEST]: Making copy of first vector.\n");
        if (numeric_vector_copy(&numbers, &copy2, false)) {
            printf("[TEST]: Passed!\n");
            printf("[TEST]: Printing vector.\n");
            numeric_vector_print(&copy2);
        } else {
            fprintf(stderr, "[TEST]: Failed!\n");
        }

        numeric_vector_free(&copy2);
    } else {
        printf("[TEST]: Failed! Couldn't allocate memory.\n");
    }

    printf("[TEST]: Clearing vector.\n");
    if (numeric_vector_clear(&numbers)) {
        printf("[TEST]: Passed!\n");
    } else {
        printf("[TEST]: Failed!\n");
    }

    printf("[TEST]: Printing vector.\n");
    numeric_vector_print(&numbers);

    numeric_vector_free(&numbers);
    return 0;
}

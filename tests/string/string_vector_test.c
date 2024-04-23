#include <stdlib.h>
#include <vector.h>

int main(void)
{
    // set_debug(true);

    StringVector names;
    printf("[TEST]: Initializing StringVector with spaces to hold 3 strings.\n");

    if (string_vector_init(&names, 3)) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        return 1;
    }

    printf("[TEST]: Adding first string to StringVector.\n");
    if (string_vector_add(&names, "John")) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        string_vector_free(&names);
        return 1;
    }

    printf("[TEST]: Adding second string to StringVector.\n");
    if (string_vector_add(&names, "Alice")) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        string_vector_free(&names);
        return 1;
    }

    printf("[TEST]: Adding third string to StringVector.\n");
    if (string_vector_add(&names, "Lucy")) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        string_vector_free(&names);
        return 1;
    }

    printf("[TEST]: Printing StringVector.\n");
    string_vector_print(&names);

    printf("[TEST]: Adding fourth string to StringVector.\n");
    if (string_vector_add(&names, "James")) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        string_vector_free(&names);
        return 1;
    }

    printf("[TEST]: Printing StringVector.\n");
    string_vector_print(&names);

    printf("[TEST]: Shrinking StringVector.\n");
    if (string_vector_shrink_to_fit(&names)) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        string_vector_free(&names);
        return 1;
    }

    printf("[TEST]: Printing StringVector.\n");
    string_vector_print(&names);

    printf("[TEST]: Getting 1st item from StringVector: %s.\n", string_vector_get_first(&names));
    printf("[TEST]: Getting 2nd item from StringVector: %s.\n", string_vector_get_at(&names, 1));
    printf("[TEST]: Getting 3rd item's length: %li.\n", string_vector_strlen(&names, 2));
    printf("[TEST]: Getting 5th item from StringVector: %s?\n", string_vector_get_at(&names, 4));
    printf("[TEST]: Getting 5th item's length: %li.\n", string_vector_strlen(&names, 4));
    printf("[TEST]: Getting last item from StringVector: %s.\n", string_vector_get_last(&names));

    printf("[TEST]: Pop-ing StringVector.\n");
    char *last = string_vector_pop(&names);

    if (last != NULL) {
        printf("[TEST]: Passed!\n");
        printf("Pop-ed item: %s.\n", last);
        free(last);
    } else {
        printf("[TEST]: Failed!\n");
    }

    printf("[TEST]: Printing StringVector.\n");
    string_vector_print(&names);

    printf("[TEST]: Adding fourth string to StringVector (again).\n");
    if (string_vector_add(&names, "Phil")) {
        printf("[TEST]: Passed!\n");
    } else {
        fprintf(stderr, "[TEST]: Failed!\n");
        string_vector_free(&names);
        return 1;
    }

    printf("[TEST]: Printing StringVector.\n");
    string_vector_print(&names);

    printf("[TEST]: Clearing vector.\n");
    if (string_vector_clear(&names)) {
        printf("[TEST]: Passed!\n");
    } else {
        printf("[TEST]: Failed!\n");
    }

    printf("[TEST]: Printing StringVector.\n");
    string_vector_print(&names);

    string_vector_free(&names);
    return 0;
}

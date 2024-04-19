#include "vector.h"

int main(void)
{
    set_debug(true);

    StringVector names;
    string_vector_init(&names, 3);
    string_vector_add(&names, "John");
    string_vector_add(&names, "Alice");
    string_vector_add(&names, "Lucy");

    printf("\nVector:\n");
    string_vector_print(&names);
    printf("\n");

    string_vector_add(&names, "James");

    printf("\nVector:\n");
    string_vector_print(&names);
    printf("\n");

    string_vector_shrink_to_fit(&names);

    printf("\nVector:\n");
    string_vector_print(&names);
    printf("\n");

    printf("1st item on vector is: %s.\n", string_vector_get_first(&names));
    printf("2nd item on vector is: %s.\n", string_vector_get_at(&names, 1));
    printf("3rd item's length on vector is: %li.\n", string_vector_strlen(&names, 2));
    printf("5th item on vector is: %s?\n", string_vector_get_at(&names, 4));
    printf("5th item's length on vector is: %li.\n", string_vector_strlen(&names, 4));
    printf("Last item on vector is: %s.\n", string_vector_get_last(&names));
    printf("\n");

    string_vector_free(&names);
    return 0;
}

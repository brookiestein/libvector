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

    string_vector_free(&names);
    return 0;
}

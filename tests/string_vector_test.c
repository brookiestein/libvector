#include "vector.h"

int main(void)
{
    set_debug(true);

    StringVector vector;
    string_vector_init(&vector, 3);

    string_vector_add(&vector, "Brayan");
    string_vector_add(&vector, "Stephanye");
    string_vector_add(&vector, "Nieves");

    printf("\nVector:\n");
    string_vector_print(&vector);
    printf("\n");

    const size_t size = 5;
    const char *other_names[size] = {
        "Pepe", "Pedrito", "Josefina",
        "Miguel", "Miguelina"
    };

    string_vector_add_array(&vector, other_names, size);

    printf("\nVector:\n");
    string_vector_print(&vector);
    printf("\n");

    string_vector_shrink(&vector);
    printf("\nVector:\n");
    string_vector_print(&vector);
    printf("\n");

    string_vector_free(&vector);
}

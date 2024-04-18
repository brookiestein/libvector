#include "vector.h"

int main(void)
{
    set_debug(true);

    NumericVector numbers;
    numeric_vector_init(&numbers, 3);
    numeric_vector_add(&numbers, 1);
    numeric_vector_add(&numbers, 2);
    numeric_vector_add(&numbers, 3);

    printf("\nVector:\n");
    numeric_vector_print(&numbers);
    printf("\n");

    numeric_vector_add(&numbers, 4);
    printf("\nVector:\n");
    numeric_vector_print(&numbers);
    printf("\n");

    numeric_vector_shrink_to_fit(&numbers);
    printf("\nVector:\n");
    numeric_vector_print(&numbers);
    printf("\n");

    numeric_vector_free(&numbers);
    return 0;
}

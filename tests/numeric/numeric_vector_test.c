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

    printf("1st item on vector is: %.2f\n", numeric_vector_get_first(&numbers));
    printf("2nd item on vector is: %.2f\n", numeric_vector_get_at(&numbers, 1));
    printf("5th item on vector is: %.2f?\n", numeric_vector_get_at(&numbers, 4));
    printf("Last item on vector is: %.2f\n", numeric_vector_get_last(&numbers));
    printf("\n");

    numeric_vector_free(&numbers);
    return 0;
}

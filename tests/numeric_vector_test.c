#include "vector.h"

int main(void)
{
    set_debug(true);

    NumericVector vector;

    numeric_vector_add(&vector, 1);
    numeric_vector_add(&vector, 2);
    numeric_vector_add(&vector, 3);
    numeric_vector_add(&vector, 4);
    numeric_vector_add(&vector, 5);
    numeric_vector_add(&vector, 6);
    numeric_vector_add(&vector, 7);
    numeric_vector_add(&vector, 8);
    numeric_vector_add(&vector, 9);
    numeric_vector_add(&vector, 10);

    numeric_vector_print(&vector);
}

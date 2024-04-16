# libvector
Vector is a C library that allows you to hold a dynamic number of elements in one container which isn't limited in how many elements you can store just like a conventional array.

**Vector** is inspired by **std::vector** from the C++'s STL library.

# Usage:
## StringVector
Let's imagine you want to store an indefinite number of names. You'd do that like:

```
StringVector names;
string_vector_add(&names, "John");
string_vector_add(&names, "Alie");
string_vector_add(&names, "Josh");
```

Here, because you haven't explicitly initialized the vector. It's automatically initialized to hold, by default, 10 strings.
But that isn't limited to that, let's add 8 strings more. That would give us a total of 11 strings, but we just have memory for 10, right?

```
string_vector_add(&names, "James");
string_vector_add(&names, "Joseph");
string_vector_add(&names, "Ninia");
string_vector_add(&names, "Miguelina");
string_vector_add(&names, "Natty");
string_vector_add(&names, "Katie");
string_vector_add(&names, "Louis");
string_vector_add(&names, "David");
```

At first glance we could think that the program will crash because we're writing into memory we don't own, but that's not true with `libvector`.
`string_vector_add()` is smart enough to detect those scenarios, and will increase the vector size 10 spaces more. Why 10? Why not just 1 space?
At the end that's what we're needing, right? That's true, but the thing is that we would be resizing the vector every time we add one more string.
To avoid that, `libvector` resizes the `StringVector` to hold 10 strings more, so that operation won't need to be performed every single time.

A `StringVector` isn't limited to add one string at a time. Let's imagine you already have an array of names. You can add that array to your `StringVector` as well.
```
char *other_names[] = { "Josefa", "Nidia", "Mon", "Natalie", "Stephanye" };
string_vector_add_array(&names, other_names);
```

In this scenario, `libvector` won't increase the `StringVector` 10 more spaces if it determines the `StringVector` needs to be bigger.
`libvector` will increase the `StringVector` as many spaces as `other_names` has items inside.

So, at the end of the day, `names`' size will be 16. Initially 3, later it had to increase 10 more: 13, occupying just 11.
And adding 5 more strings through `string_vector_add_array()` will make `libvector` to increase `names`' size to adjust itself to be able to hold those 5 more strings, resulting in 16 strings.

If you're in a situation where `libvector` had to increase your `StringVector` to hold 10 more strings, but your `StringVector` is holding, for example, 3 strings, you can
use `string_vector_shrink()` to tell `libvector` to make its size equal to the items it holds.
### For example:
```
StringVector names;
string_vector_add(&names, "John Doe");
string_vector_add(&names, "Alice");
string_vector_add(&names, "James");

string_vector_print(&names);
string_vector_shrink(&names);
string_vector_print(&names);
```

Here we're using `string_vector_print()` to print the `StringVector`, it will print every string the `StringVector` holds, but also it tells you how many items it holds, and how many items it **can** hold.
Then we're shrinking `names` to adjust its size to the number of items it's currently holding, and finally printing it again for you to be sure the operation was performed successfully.

## NumericVector
`NumericVector` behaves almost the same as `StringVector`. By default it works with `double`s, so it'd be pretty easy to cast to other numeric types, e.g., `int`.

The operations you can perform in a vector, be it `StringVector` or `NumericVector` starts with its name. You can see every operation we performed in `StringVector` started with `string_vector_*`.
That's exactly the same as for `NumericVector`.

When you finish working with `NumericVector` or `StringVector`, you must call: `numeric_vector_free(&numeric_vector)` or `string_vector_free(&string_vector)`.

# List of operations you can perform in every vector:
| Vector        | Operation                                         | Description                                                                                                                   |
|---------------|---------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------|
| StringVector  | string_vector_init(&vector, initial_size)         | Initalizes vector to hold `initial_size` items.                                                                               |
| StringVector  | string_vector_add(&vector, string)                | Adds `string` to `vector`. Resizes `vector` to hold 10 more strings if needed.                                                |
| StringVector  | string_vector_add_array(&vector, strings, n)      | Adds array of strings `strings` containing `n` strings to `vector`. Resizes `vector` to hold `n` items more if needed.        |
| StringVector  | string_vector_free(&vector)                       | Frees `vector`s allocated memory. MUST be called when you're done working with the vector.                                    |
| StringVector  | string_vector_resize(&vector, spaces)             | Resizes `vector` to hold `spaces` more, in addition to the ones it currently holds.                                           |
| StringVector  | string_vector_shrink(&vector)                     | Shrinks `vector` to adjust its size the the items it currently holds.                                                         |
| StringVector  | string_vector_print(&vector)                      | Prints all the strings held by `vector`. Also tells how many items are held, and how many items can be held without resizing. |
| NumericVector | numeric_vector_init(&vector, initial_size         | Initalizes vector to hold `initial_size` items.                                                                               |
| NumericVector | numeric_vector_add(&vector, number)               | Adds `number` to `vector`. Resizes `vector` to hold 10 more strings if needed.                                                |
| NumericVector | numeric_vector_resize(&vector, spaces)            | Resizes `vector` to hold `spaces` more, in addition to the ones it currently holds.                                           |
| NumericVector | numeric_vector_free(&vector)                      | Frees `vector`s allocated memory. MUST be called when you're done working with the vector.                                    |
| NumericVector | numeric_vector_print(&vector)                     | Prints all the strings held by `vector`. Also tells how many items are held, and how many items can be held without resizing. |

`StringVector` has some more operations than `NumericVector`. They're pending to be added to `NumericVector`.

//
//  ArrayOperation.c
//  20210906_buildDennielCLibrary
//
//  Created by Yen on 2021/9/7.
//

#include "ArrayOperation.h"

/* Initialise an empty array */
void Array_Init(CTypeArray *array)
{
    float *float_pointer;

    float_pointer = (float *)malloc(sizeof(float));

    if (float_pointer == NULL)
    {
        printf("Unable to allocate memory, exiting.\n");
        free(float_pointer);
        exit(0);
    }
    else
    {
        array->array = float_pointer;
        array->size = 0;
    }
}

/* Dynamically add to end of an array */
void Array_Add(CTypeArray *array, float item)
{
    float *float_pointer;

    array->size += 1;

    float_pointer = (float *)realloc(array->array, array->size * sizeof(float));

    if (float_pointer == NULL)
    {
        printf("Unable to reallocate memory, exiting.\n");
        free(float_pointer);
        exit(0);
    }
    else
    {
        array->array = float_pointer;
        array->array[array->size-1] = item;
    }
}

/* Delete from a dynamic array */
void Array_Delete(CTypeArray *array, float index)
{
    int i;
    CTypeArray temp;
    float *float_pointer;

    Array_Init(&temp);

    for(i=index; i<array->size; i++)
    {
        array->array[i] = array->array[i + 1];
    }

    array->size -= 1;

    for (i = 0; i < array->size; i++)
    {
        Array_Add(&temp, array->array[i]);
    }

    float_pointer = (float *)realloc(temp.array, temp.size * sizeof(float));

    if (float_pointer == NULL)
    {
        printf("Unable to reallocate memory, exiting.\n");
        free(float_pointer);
        exit(0);
    }
    else
    {
        array->array = float_pointer;
    }
}

/* Free an array */
void Array_Free(CTypeArray *array)
{
  free(array->array);
  array->array = NULL;
  array->size = 0;
}

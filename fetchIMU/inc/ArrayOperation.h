//
//  ArrayOperation.h
//  20210906_buildDennielCLibrary
//
//  Created by Yen on 2021/9/7.
//

#ifndef ArrayOperation_h
#define ArrayOperation_h

#include <stdio.h>
#include <stdlib.h>
#include "ArrayStruct.h"
#endif /* ArrayOperation_h */

void Array_Init(CTypeArray *array);
void Array_Add(CTypeArray *array, float item);
void Array_Delete(CTypeArray *array, float index);
void Array_Free(CTypeArray *array);

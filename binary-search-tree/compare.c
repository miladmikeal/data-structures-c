#include <stdio.h>
#include <assert.h>
#include "bst.h"
#include "structs.h"

/*----------------------------------------------------------------------------
 very similar to the compareTo method in java or the strcmp function in c. it
 returns an integer to tell you if the left value is greater then, less then, or
 equal to the right value. you are comparing the number variable, letter is not
 used in the comparison.

 if left < right return -1
 if left > right return 1
 if left = right return 0
 */
int compare(TYPE left, TYPE right)
{
    struct data *l = left;
    struct data *r = right;
    if (l->number < r->number)
        return -1;
    else if (l->number > r->number)
        return 1;
    else
        return 0;
}

void print_type(TYPE curval)
{
    struct data *val = curval;
    printf("Current value: %d", val->number);
}

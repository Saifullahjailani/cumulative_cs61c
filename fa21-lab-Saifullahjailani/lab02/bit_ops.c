#include <stdio.h>
#include "bit_ops.h"

/* Returns the Nth bit of X. Assumes 0 <= N <= 31. */
unsigned get_bit(unsigned x, unsigned n) {
    /* YOUR CODE HERE */
    /*
    Description:
    * First shift the nth bit all the way to the left, the all the ways to the right.
    */ 
    return (x >> n) & 1; 
}

/* Set the nth bit of the value of x to v. Assumes 0 <= N <= 31, and V is 0 or 1 */
void set_bit(unsigned *x, unsigned n, unsigned v) {
    /*
    Description:
    * Take the nth bit then shift it back to its position, by Xoring with the nth bit we zero out 
    * that bit only and ones doesnt change. Then we shift V to nth positoin and add them. 
    */
    unsigned a = *x &(~(1 << n));
    *x = a | (v << n);

}

/* Flips the Nth bit in X. Assumes 0 <= N <= 31.*/
void flip_bit(unsigned *x, unsigned n) {
    /*
    * We take the nth bit and invert it. since there are other bits allso which will be 1 after the inversion
    * we take onely the first bit. And then we just set the nth bit of v with the inverted bit. 
    */
    unsigned bit = ~get_bit(*x, n);
    bit = get_bit(bit, 0);
    set_bit(x, n, bit);
}


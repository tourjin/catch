#include <stdio.h>

typedef struct set {
    int a;
    int b;
    float c;
} set;
void sub(set * my_set){
  my_set->a = 1;
  my_set->b = 2;
  my_set->c = .9;
}
set my_set;
int main() {
    // Initialize all to zero
//    set my_set = {0}; 
    
    printf("a: %d, b: %d, c: %.1f\n", my_set.a, my_set.b, my_set.c);
    // Output: a: 0, b: 0, c: 0.0
   sub(&my_set);
    printf("a: %d, b: %d, c: %.1f\n", my_set.a, my_set.b, my_set.c);
    return 0;
}

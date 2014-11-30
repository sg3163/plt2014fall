#include <stdio.h>

int main(void) {

    int a[2][3] ;
    
    a[1][1] = 80;
    
    if(80 == *( *(a+1) + (1))) {
        printf("Address MAtches");
    } else {
        printf("Address doesn't match");
    }

    return 0;
}

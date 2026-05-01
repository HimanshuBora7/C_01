//write a program to calculate simple interest

#include <stdio.h>
int main(){

    int p,n;
    float r,si;
    // p =100;
    // n=3;
    // r =8.5;
    printf("enter the value for p");
    scanf("%d",&p);
    
    si = (p*r*n)/100;
    printf("%f \n",si);
    return 0;
}
//enter the entries in an array and calculate the average of the whole array 

#include<stdio.h>

void writeArray(int arr [], int size); 

int main (){


    int avg, sum =0;
    int marks[5];

  writeArray(marks ,5 );

    for(int j=0;j<5;j++){
    printf("%d \n",marks[j]);
    sum+=marks[j];
}
avg = sum/4;
printf("\n average %d",avg);
    return 0;
}

void writeArray(int arr[], int size){
      for(int i = 0 ; i < size ;i++){
        printf("enter the entry for %d\n ", i+1);
        scanf(" %d",&arr[i]);
    }
}
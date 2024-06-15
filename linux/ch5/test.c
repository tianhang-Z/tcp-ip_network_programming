#include<stdio.h>
int number;
int main ()
{ 
    while( scanf("%d",&number) != 1){
        while(getchar()!='\n'){ 
            printf("enter a int number.\n");
        }
    }
    printf("%d.\n",number);
}

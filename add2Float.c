#include <stdio.h>
#include <unistd.h>
#include "addWrapper.h"


int main(int argc,char *argv[]){
	float num1,num2,result;

	/*Take user input of two float numbers*/
	printf("Enter float 1 and float 2\n");
	scanf("%f %f",&num1,&num2);

	/*Calling add_sycall function present in addWrapper.h It returns result of addition of the float numbers*/
	result=add_syscall(num1,num2); 
	printf("The result of adding %f with %f is %f\n",num1,num2,result);
	
	return 0;

}

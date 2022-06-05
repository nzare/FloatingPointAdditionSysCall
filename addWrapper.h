#ifndef __ADD_WRAPPER_H_
#define __ADD_WRAPPER_H_
#endif


#include<stdio.h>


/*Union to store datatypes in same memory location*/
/*Struct to store float in ieee single precision format*/
typedef union { 

	float f; 
	struct
	{ 

		// First manitssa/fractional part is stored, then exponent and then sign 
		unsigned int fraction : 23; 
		unsigned int exponent : 8; 
		unsigned int sign : 1; 

	} float_union; 
} float_ieee; 


/*A function that returns power of first number raised to second number for both non-negative numbers*/

int power(int x,int y){
	if(x==0){
		return 0;
	}
	if(y==0){
		return 1;
	}
	int ans=1;
	
	while(y--){

		ans*=x;
	}
	
	return ans;

}

/*Function that returns sign from the given struct of a float number*/
int float_sign(float_ieee f_num){
	return f_num.float_union.sign;
}


/*Function that returns exponent from the given struct of a float number*/
int float_exponent(float_ieee f_num){
	return f_num.float_union.exponent;
}


/*Function that returns fraction from the given struct of a float number*/
int float_fraction(float_ieee f_num){
	return f_num.float_union.fraction;
	
}

/*Function to store binary representation of number in an array */
void store_binary(int x,int arr[],int start){
	int i=31-start;
	while(x>0){
		arr[i]=x%2;
		x=x/2;
		i--;
	}
}

/*Function to convert binary representation stored in array to integer*/
unsigned int convert_Int(int arr[], int low, int high) 
{ 
	unsigned f = 0, i; 
	for (i = high; i >= low; i--) { 
		f = f + arr[i] * power(2, high - i); 
	} 
	return f; 
} 


float add_syscall(float f1,float f2){

	
	float_ieee num1,num2; 

	/*First number is the greater number*/
	if(f1>f2){
		num1.f=f1;
		num2.f=f2;
	}
	else{
		num1.f=f2;
		num2.f=f1;
	}


	/*An array to store single precision represntation of addition of float numbers*/
	int arr[32]={0};

	/*Variable to check if exponent needs to be chanaged after addition if after addition fraction part overflow*/
	int temp=0;
	

	long int res; //Store output of system call

	/*Or of sign bit of both floats. If any one of them is 1,(that is number is negative) this is set to one to return error from system call */

	int sign=float_sign(num1)|float_sign(num2);


	int eq_exp=0;  //Keep track if exponents are equal
	

	/*if either of the number is negaitve return -10.000000 as error value*/
	if(sign==1){

		return -10;
	}
	
	/*if exponent of float is 255 and fraction is non-zero , it is NAN*/

	/*Retuns -30.000000 if second number is NAN and -40.000000 if first number is NAN*/
	else if(float_exponent(num1)==255 && float_fraction(num1)!=0){
		return -30;
	}
	else if(float_exponent(num2)==255 && float_fraction(num2)!=0){
		return -40;
	}	

	/*if exponent is 255 and fraction is zero (exponent 255 and fraction non-zero already checked), number is infinty*/

	/*Returns -50.000000 if either of the number is infinite*/
	else if(float_exponent(num1)==255 || float_exponent(num2)==255){
		return -50;

	}
	
	/*For valid positive float numbers: System call (System call number is 434). It adds fraction part of both float numbers */

	res=syscall(434,float_fraction(num1),float_fraction(num2),float_exponent(num1),float_exponent(num2));
	
	if(res<0){
		//Returned by system call in case of error: overflow after addition
		
		/* Returns: -1.000000 if addition overflows */
		return res;		 
	}


	if(float_exponent(num1)==float_exponent(num2)){   //For equal exponents
		
		/*Store ieee format of addition in array*/
		store_binary(res/2,arr,0);  //fraction part
		eq_exp=1;
		store_binary(float_exponent(num1)+1,arr,23); //exponent
	}
	else{						// For un-equal exponents
		
		if(res>=power(2,23)){
			temp=1;  // Adjust exponent if fraction overflows
		}

		/*Store ieee format of addition in array*/
		store_binary(res,arr,0);  //fraction part
		
		store_binary(float_exponent(num1)+temp,arr,23);  //exponent


	}
	float_ieee ans; 

	/*Convert fraction part stored in array to decimal representation of integer*/
	unsigned ans_temp = convert_Int(arr, 9, 31); 

	// Adjust fraction part as per final exponent 

	if(eq_exp==1){
		ans.float_union.fraction = ans_temp; 
	}
	else{
		ans.float_union.fraction = ans_temp/power(2,temp);  // fraction bits will be right shifted (binary representation) that is divided by 2 if fraction overflows
	}


	/*Convert exponent part stored in array to decimal representation of integer*/
	ans_temp = convert_Int(arr, 1, 8); 

	
	ans.float_union.exponent = ans_temp;  // Assign as per the structure of float

	// Assign sign bit 
	ans.float_union.sign = arr[0]; 
	
	
	return ans.f;
}




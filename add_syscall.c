#include<linux/kernel.h>
#include<linux/types.h>
#include <linux/syscalls.h>
#include "add_syscall.h"

SYSCALL_DEFINE4(add_syscall, int, f1, int, f2 ,int, e1 ,int, e2){
	
	int exponent_diff;
	int m;
	
	exponent_diff=e1-e2;

	/*Shift fraction of smaller number (number 2 is always smaller, this is ensured by wrapper function in user space)*/
	f2=f2 >> exponent_diff;
	
	/*Check overflow on addition. If both exponents are equal and equal to 254, then their addition is will
	 result in exponent=255 which is either NAN or infinity. That's why return -1 */

	if(exponent_diff==0 && e1==254){
		return -1;		
	}

	/*Check overflow on addition. If both exponents are unequal and larger exponent is equal to 254 and addition of fractions is greater than (2^23), then their addition is will
	 result in exponent=255 which is either NAN or infinity. That's why return -1 */

	else if(exponent_diff!=0 && e1==254 && (f1+f2)>=8388608){
		return -1;
	}
	
	/*For valid positive floating point numbers, first adjust fraction (1 of 1.fraction form after shifting fraction using or)
	of second number according to difference of exponents and then return addition of fractional parts*/	
	else {
		if(exponent_diff>23 ||exponent_diff==0){
			m=0;
		}
		else{
			m=1 << (23-exponent_diff);
		}
		f2=f2 | m; 
		return f1+f2;
	
	    }	

}

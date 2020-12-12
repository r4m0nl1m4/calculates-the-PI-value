
//Main for "pi" C++ application
//Created by r4m0nl1m4 14/10/2020

//Guard
#ifndef PI_H
#define PI_H

/*
 * Generating PI Value Using Leibnitz Infinite Series
 */
double getPIValueByLeibniz(unsigned long int i){ 
	return 1+i/(2*i+1)*getPIValueByLeibniz(i+1); 
}

/*
 * Generating PI Value Using Gregory-Leibnitz Infinite Series
 */
double getPIValueByGregoryLeibniz(unsigned long int n){
	double pi = 4;
	int flag = 0;
	for (unsigned long int i=3; i<n; i+=2 ){
		if(flag) pi += 4/i;
		else pi -= 4/i;
		flag = !flag;
    }
    return pi;
}

/*
 * Generating PI Value Using Nilakantha Series
 */
double getPIValueByNilakantha(unsigned long int  n){
	double pi = 3;
	int flag = 1;
	for (unsigned long int i=2; i<n; i+=2 ){
		if(flag) pi += (4/(i*(i+1)*(i+2)));
		else pi -= (4/(i*(i+1)*(i+2)));
		flag = !flag;
    }
    return pi;
}

/*
 * Generating PI Value Using Integration
 */
double getPIValueByIntegration(double n){
	double step, sum, x, pi;
	step = 1/n;
	sum = 0;
	for(double i=1; i<=n; i++ ){
		x = (i-0.5)*step;
		sum = sum + 1/(1+x*x);
    }
    pi = 4*sum*step;
    return pi;
}

#endif
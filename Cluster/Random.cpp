#include <iostream>
#include <cstdlib>  //rand, RAND_MAX
#include <ctime>
#include <math.h>

using namespace std;

//Marsaglia
double uniform_distribution_Marsaglia(int M, int N) {

	double y1, y2, r, result, temp;
	do{
    	y1 = M + ((double)rand()/ RAND_MAX)*(N-M);
   		y2 = M + ((double)rand()/ RAND_MAX)*(N-M);
   		r = pow(y1,2)+ pow(y2,2);
   	}while(r < 1);
   	
   	temp = ((-2)*log(r))/(r); 		//temp = ((-2)*log(r))/(r);
   	if (temp < 0)
   		temp = temp*(-1);
   	result = sqrt(temp)*y1;
  //cout << "Marsaglia : " << result << endl;
	return result;
}

int uniform_distribution(int M, int N){
	return (M + ((double)rand()/ (RAND_MAX + 1.0))*(N-M+1));    
}

double uniform_distribution_real(int M, int N){
	return (M + ((double)rand()/ (RAND_MAX))*(N-M));    
}

float uniform_distribution_float(float M, float N){
	return (M + ((double)rand()/ (RAND_MAX))*(N-M));    
}

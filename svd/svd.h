#ifndef _SVD_H
#define _SVD_H

#include <string.h>
#include <cmath>
#include <math.h>
#include <stddef.h>
#include<string.h>
#include<vector>
#include<sstream>
#include <iostream>

using std::cin; using std::cout; using std::endl;


#define THREASHOLD 1e-8
struct message_value
      {
              double matrixCol[200];
              double identityCol[200];
      };
double sign(double x)
{
	if(x>0)
		return 1;
	else 
		return -1;
}

double vectorProduct(double *vector1,double *vector2,unsigned long long len)
{
	double result = 0;
	unsigned long long  i;
	for(i = 0 ; i < len ;++i)
	{
		result+=vector1[i] *vector2[i];
	}
	return result;
};
void orthogonal(struct message_value &value1,struct message_value &value2,unsigned long long rowNum,unsigned long long colNum)
{
	double ele = vectorProduct(value1.matrixCol,value2.matrixCol,rowNum);
	if(fabs(ele) < THREASHOLD)
		return;
	double ele1 = vectorProduct(value1.matrixCol,value1.matrixCol,rowNum);
	double ele2 = vectorProduct(value2.matrixCol,value2.matrixCol,rowNum);
	
	double tao =(ele1-ele2)/(2*ele);
    double tan=sign(tao)/(fabs(tao)+sqrt(1+pow(tao,2)));
    double cos=1/sqrt(1+pow(tan,2));
    double sin=cos*tan;	
	unsigned long long index ;
	for(index = 0 ; index < rowNum; ++index)
	{
		double val1= (value1.matrixCol)[index]*cos +(value2.matrixCol)[index]*sin;
		double val2 =(value2.matrixCol)[index]*cos -(value1.matrixCol)[index]*sin;
		(value1.matrixCol)[index] = val1;
		(value2.matrixCol)[index] = val2;
	}
	for(index = 0;index <colNum;++index)
	{
		double val1= (value1.identityCol)[index]*cos +(value2.identityCol)[index]*sin;
		double val2 =(value2.identityCol)[index]*cos -(value1.identityCol)[index]*sin;
		(value1.identityCol)[index] = val1;
		(value2.identityCol)[index] = val2;
   }
};


void normalize(double *b, double *u, double *s, int rows) {
	double norm = sqrt(vectorProduct(b,b,rows));
	*s = norm;
	
	if (fabs(norm) < THREASHOLD)
		for (int i = 0; i<rows; ++i)
			u[i] = 0;
	else
		for (int i = 0; i<rows; ++i)
			u[i] = b[i] / norm;
}
#endif




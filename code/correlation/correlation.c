#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../contour/array.h"

double mean(Array array) {
    double sum = getArraySum(array);
    return sum/len;
}

void shift(Angle &x) {
  double firstElement = x.data[0];
  for(idx = 0; idx < x.length - 1; idx++) {
    x.data[idx] = x.data[idx + 1];
  }
  x.data[x.length - 1] = firstElement;
}

double steadyStateCorrelator(Array x, Array y) {
    double sum = 0;
    for (int i = 0; i < x.length; i++) {
      sum += x.data[j]*y.data[j];
    }
    sum;
}

/* TODO: Update this function!. */
void fastSteadyStatePearsonCorrelator(Array x, Array y) {
    /* Calculate the correlation of the current shift */
    double r = steadyStateCorrelator(lenX, x, lenY, y, xy);
    /* compute Pearson coefficients by correcting correlation */
    double sx = 0, sx2 = 0;
    double sy = 0, sy2 = 0;
    for (int i=0; i < lenY; i++) {
      sx += x[i]; sx2 += x[i]*x[i];
      sy += y[i]; sy2 += y[i]*y[i];
    }
    double my = sy/lenY;
    double vary = sqrt(sy2 - sy*sy/lenY); 
    double varx = sqrt(sx2 - sx*sx/lenY);
    for (int i=0; i < lenXY; i++) {
      xy[i] = (xy[i] - my*sx)/(varx*vary);
      sx = sx - x[i] + x[i+lenY];
      sx2 = sx2 - x[i]*x[i] + x[i+lenY]*x[i+lenY];
      varx = sqrt(sx2 - sx*sx/lenY);
    }
}

double correlation(Array x, Array y) {
  double pearsonCoeff, pearsonMaxCoeff = 0;

  if(x.length != y.length) {
    exit(-1);
  }

  for (shift = 0; shift < x.length; shift++) {
    shift(&x);
    pearsonCoeff = fastSteadyStatePearsonCorrelator(x, y);
    if(pearsonCoeff > pearsonMaxCoeff) {
      pearsonMaxCoeff = pearsonCoeff;
    }
  }

  return pearsonMaxCoeff;
}
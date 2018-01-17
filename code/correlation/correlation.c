#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../contour/array.h"
#include "correlation.h"

double mean(Array x) {
    double sum = getArraySum(x);
    return sum/x.length;
}

void shift(Array *x) {
  double firstElement = x->data[0];
  for(int idx = 0; idx < x->length - 1; idx++) {
    x->data[idx] = x->data[idx + 1];
  }
  x->data[x->length - 1] = firstElement;
}

double steadyStateCorrelator(Array x, Array y) {
    double sum = 0;
    for (int i = 0; i < x.length; i++) {
      sum += x.data[i]*y.data[i];
    }
    return sum;
}

double fastSteadyStatePearsonCorrelator(Array x, Array y) {
    /* Calculate the correlation of the current shift */
    double r = steadyStateCorrelator(x, y);

    /* compute Pearson coefficients by correcting correlation */
    double sx = 0, sx2 = 0;
    double sy = 0, sy2 = 0;
    for (int i = 0; i < y.length; i++) {
      sx += x.data[i]; sx2 += x.data[i]*x.data[i];
      sy += y.data[i]; sy2 += y.data[i]*y.data[i];
    }
    double my = sy/y.length;
    double vary = sqrt(sy2 - sy*sy/y.length); 
    double varx = sqrt(sx2 - sx*sx/y.length);
    
    return (r - my*sx)/(varx*vary);
}

/* Performs the circular correlation between 2 arrays with the same length. */
double correlation(Array x, Array y) {
  double pearsonCoeff, pearsonMaxCoeff = 0;

  if(x.length != y.length) {
    fprintf(stderr, "The 2 arrays do not have the same length!\n\n");
    exit(-1);
  }

  for (int shiftIdx = 0; shiftIdx < x.length; shiftIdx++) {
    shift(&x);
    pearsonCoeff = fastSteadyStatePearsonCorrelator(x, y);
    if(pearsonCoeff > pearsonMaxCoeff) {
      pearsonMaxCoeff = pearsonCoeff;
    }
  }

  return pearsonMaxCoeff;
}
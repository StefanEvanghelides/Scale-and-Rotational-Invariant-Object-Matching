#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../array/array.h"
#include "interpolation.h"

/* Stretch array x to have the same size as the base array. */
void nonUniformInterpolation(Array *base, Array *x) {
	// Initialize new array
	Array stretched; initArray(&stretched);

	// Add new values:
	// for Double step = len(base) / len(x), make sure that every time step reaches 1,
	//      add an interpolated value into the array
	double baseStep = (double) base->length / x->length - 1;
	double currentStep = baseStep;
	for(int idx=0; idx < x->length; idx++) {
		while(currentStep >= 1) {
			// Add the interpolated value
			if(idx > 0) addElement(&stretched, (x->data[idx-1] + x->data[idx]) / 2);
			else addElement(&stretched, (x->data[idx] + x->data[x->length - 1]) / 2);	

			currentStep -= 1; // Set currentStep with 1 value lower
		}
		addElement(&stretched, x->data[idx]);
		currentStep += baseStep;
	}

	// Make x = stretched
	for(int i = x->length; i<base->length; i++) {
		addElement(x, 0); //add dummy values to match the length
	}
	for(int i=0; i < x->length; i++) {
		x->data[i] = stretched.data[i];
	}
}

double getSplineElement(double step, Array a, Array b, Array c, Array d) {
	double element = 0.0;
	int splineIdx = (int) step;
	double A = a.data[splineIdx];
	double B = b.data[splineIdx];
	double C = c.data[splineIdx];
	double D = d.data[splineIdx];

	// NOTE: check the parentheses! step - splineIdx (-1??)
	element = A + B*(step - splineIdx) + C*pow(step - splineIdx, 2) + D*pow(step - splineIdx, 3);

	return element;
}

/* Compute the components of the cubic spline. */
void computeCubicSplines(Array *a, Array *b, Array *c, Array *d) {

    /** Step 0 */
    int n, i, j;
    double *h, *A, *l, *u, *z;

    n = a->length - 1;
    
    h = calloc(n, sizeof(double));
    A = calloc(n, sizeof(double));
    l = calloc(n+1, sizeof(double));
    u = calloc(n+1, sizeof(double));
    z = calloc(n+1, sizeof(double));

    /** Step 1 */
    // NOTE: Here was h[i] = x[i+1] - x[i];
    for (i = 0; i < n; i++) h[i] = 1;

    /** Step 2 */
    for (i = 1; i < n; i++)
        A[i] = 3 * (a->data[i + 1] - a->data[i]) / h[i] - 3 * (a->data[i] - a->data[i - 1]) / h[i - 1];

    /** Step 3 */
    l[0] = 1;
    u[0] = 0;
    z[0] = 0;

    /** Step 4 */
    for (i = 1; i < n; i++) {
    	// NOTE: the first element of l was
    	// 2 (x[i+1] - x[i-1])
        l[i] = 4 - h[i - 1] * u[i - 1];
        u[i] = h[i] / l[i];
        z[i] = (A[i] - h[i - 1] * z[i - 1]) / l[i];
    }

    /** Step 5 */
    l[n] = 1;
    z[n] = 0;
    c->data[n] = 0;

    /** Step 6 */
    for (j = n - 1; j >= 0; j--) {
        c->data[j] = z[j] - u[j] * c->data[j + 1];
        b->data[j] = (a->data[j + 1] - a->data[j]) / h[j] - h[j] * (c->data[j + 1] + 2 * c->data[j]) / 3;
        d->data[j] = (c->data[j + 1] - c->data[j]) / (3 * h[j]);
    }

    free(A);
    free(h);
    free(l);
    free(u);
    free(z);
}


void interpolate(Array *base, Array *x) {
	// Mode 1: nonUniformInterpolation
	//nonUniformInterpolation(base, x);

	// Mode 2: Spline/lanczos
	Array b = copyArray(*x); popElement(&b);
	Array c = copyArray(*x);
	Array d = copyArray(*x); popElement(&d);
	computeCubicSplines(x, &b, &c, &d);

	double baseStep = (double) x->length / base->length;
	double currentStep = baseStep;

	Array newX; initArray(&newX);
	for(int i=0; i < base->length; i++) {
		double element = getSplineElement(currentStep, *x, b, c, d);
		addElement(&newX, element);
		currentStep += baseStep;
	}

	freeArray(*x);
	*x = copyArray(newX);

	freeArray(newX);
	freeArray(b);
	freeArray(c);
	freeArray(d);

	// Scale up/down?
}

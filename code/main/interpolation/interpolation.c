#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../array/array.h"
#include "interpolation.h"

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

/* Get the interpolated value from the splines. */
double getSplineElement(double x, Array a, Array b, Array c, Array d) {
	double element = 0.0;
	int splineIdx = (int) x;
	double A = a.data[splineIdx];
	double B = b.data[splineIdx];
	double C = c.data[splineIdx];
	double D = d.data[splineIdx];

	// NOTE: check the parentheses! step - splineIdx (-1??)
	element = A + B*(x - splineIdx) + C*pow(x - splineIdx, 2) + D*pow(x - splineIdx, 3);

	return element;
}


void cubicSplineScaleUp(Array *base, Array *x) {
	Array b = copyArray(*x); popElement(&b);
	Array c = copyArray(*x);
	Array d = copyArray(*x); popElement(&d);
	computeCubicSplines(x, &b, &c, &d);

	double baseStep = (double) x->length / base->length;
	double currentStep = baseStep;

	Array newX; 
	initArray(&newX);
	
	addElement(&newX, base->data[0]); // start with the first element
	for(int i=1; i < base->length; i++) {
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
}

void cubicSplineScaleDown(Array *base, Array *x) {
	Array b = copyArray(*base); popElement(&b);
	Array c = copyArray(*base);
	Array d = copyArray(*base); popElement(&d);
	computeCubicSplines(base, &b, &c, &d);

	double baseStep = (double) base->length / x->length;
	double currentStep = baseStep;

	Array newBase; 
	initArray(&newBase);

	for(int i=0; i < x->length; i++) {
		double element = getSplineElement(currentStep, *base, b, c, d);
		addElement(&newBase, element);
		currentStep += baseStep;
	}

	freeArray(*base);
	*base = copyArray(newBase);

	freeArray(newBase);
	freeArray(b);
	freeArray(c);
	freeArray(d);
}


void linearSpline(Array *base, Array *x) {
	double left, right, A, B;

	Array a; initArray(&a);
	Array b; initArray(&b);

	// Computing Linear Splines
	for(int i=1; i < x->length; i++) {
		left = x->data[i-1];
		right = x->data[i];

		A = right - left;
		B = right - A * i;

		addElement(&a, A);
		addElement(&b, B);
	}

	// Interpolation
	Array newX; 
	initArray(&newX);

	double baseStep = (double) x->length / base->length;
	double currentStep = baseStep;

	addElement(&newX, base->data[0]); // start with the first element
	for(int i=1; i < base->length; i++) {
		int splineIdx = (int) currentStep;
		A = a.data[splineIdx];
		B = b.data[splineIdx];

		double element = A * currentStep + B;
		
		addElement(&newX, element);
		currentStep += baseStep;
	}

	freeArray(*x);
	*x = copyArray(newX);


	freeArray(a);
	freeArray(b);
}

/* Performs one of the available interpolation methods. */
void interpolate(Array *base, Array *x) {
	// Mode 1: Linear Spline
	//linearSpline(base, x);

	// Mode 2: CubicSpline - scaling up
	//cubicSplineScaleUp(base, x);
	

	// Mode 3: CubicSpline- scaling down
	cubicSplineScaleDown(base, x);

}

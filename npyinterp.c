
#include<stdio.h>
#include<Python.h>
#include<numpy/arrayobject.h>
#include<assert.h>
npy_intp binary_search(const double key, const double arr [], const npy_intp len)
{
    npy_intp imin = 0;
    npy_intp imax = len;

    if (key > arr[len - 1]) {
        return len;
    }
    while (imin < imax) {
        npy_intp imid = imin + ((imax - imin) >> 1);
        if (key >= arr[imid]) {
            imin = imid + 1;
        }
        else {
            imax = imid;
        }
    }
    return imin - 1;
}

#define IFVERBOSE if(0)
#define adouble double
#define bdouble double

int interpolate(const void * leftp, const void * rightp, void * zp, int m,
	const void * xp, const void * yp, int n) {
	int i;
	int j;
	int k;
	const adouble * left = (adouble*) leftp;
	const adouble * right = (adouble*) rightp;
	const bdouble * x = (bdouble*) xp;
	const bdouble * y = (bdouble*) yp;
	adouble * z = (adouble*) zp;
	double yleft, yright;
	double wleft, wright;
	/* find start */
	if (left[0] < x[0]) {
		fprintf(stderr, "left (%f) is below of x-range(%f...)\n", left[0], x[0]);
		return 1;
	}
	j = binary_search(left[0], x, n);
	IFVERBOSE printf("binary search found %i <%f|%f|%f>\n", j, x[j], left[0], x[j+1]);
	k = j + 1;
	if (j + 1 >= m) {
		fprintf(stderr, "left (%f) is above of x-range(...%f)\n", left[0], x[m-1]);
		return 1;
	}
	
	/* assumptions: 
	 * left > right 
	 * x,left,right are increasing
	 */
	 
	/* for each left/right, 
	 * set z to the corresponding y-value */
	for (i = 0; i < m; i++) {
		IFVERBOSE printf("looking for %f..%f (%d of %d)\n", left[i],right[i], i, m);
		for(;left[i]  > x[j+1] && j < n; j++) {
			IFVERBOSE printf("  moved left  to %d: %f\n", j, x[j]);
		}
		for(;right[i] > x[k+1] && k < n; k++) {
			IFVERBOSE printf("  moved right to %d: %f\n", k, x[k]);
		}
		if (k == n || j == n) {
			fprintf(stderr, "box (%f..%f) is above of x-range (...%f)\n", left[i], right[i], x[k-1]);
			return 1;
		}
		
		/* interpolate for left  between x[j], x[j+1] and
		 *             for right between x[k], x[k+1] */
		
		wleft  = (left[i]  - x[j]) / (x[j+1] - x[j]);
		wright = (right[i] - x[k]) / (x[k+1] - x[k]);
		IFVERBOSE printf(" at <%f|%f|%f> weight %f\n", x[j],left[i], x[j+1], wleft);
		IFVERBOSE printf(" at <%f|%f|%f> weight %f\n", x[k],right[i],x[k+1], wright);
		/* y-values are samples of the integral. */
		yleft  = (y[j+1] - y[j]) * wleft  + y[j];
		yright = (y[k+1] - y[k]) * wright + y[k];		
		IFVERBOSE printf(" interpolating <%f|%f|%f>\n", y[j], yleft,  y[j+1]);
		IFVERBOSE printf(" interpolating <%f|%f|%f>\n", y[k], yright, y[k+1]);
		z[i] = yright - yleft;
	}
	return 0;
}

double trace2(const void * indatav, int rowcount, int colcount) {
    //void cfun(const double * indata, int rowcount, int colcount, double * outdata) {
    const double * indata = (double *) indatav;
    int n = rowcount;
    int i;
    double sum = 0.;
    if (colcount < n)
    	n = colcount;
    printf("Here we go!\n");
    for (i = 0; i < n; ++i) {
        sum += indata[i * rowcount + i];
    }
    printf("Done!\n");
    return sum;
}

/*
double trace(PyArrayObject * array) {
	double sum;
	int i, n;

	printf("array: %p -- %c\n", array, *(char*)array);
	if (array->nd != 2 || array->descr->type_num != PyArray_DOUBLE) {
		PyErr_SetString(PyExc_ValueError,
		"array must be two-dimensional and of type float");
		return -1;
	}

	n = array->dimensions[0];
	if (n > array->dimensions[1])
		n = array->dimensions[1];

	printf("dimensions: %i %i\n", (int)array->dimensions[0], (int)array->dimensions[1]);
	sum = 0.;
	for (i = 0; i < n; i++) {
		printf("  accessing %i\n", i);
		sum += *(double *)(array->data + i*array->strides[0] + i*array->strides[1]);
	}
	return sum;
}*/


/*
npy_intp PyArray_CountNonzero(PyArrayObject* self)
{
    PyArray_NonzeroFunc* nonzero = PyArray_DESCR(self)->f->nonzero;

    NpyIter* iter;
    NpyIter_IterNextFunc *iternext;
    char** dataptr;
    npy_intp* strideptr,* innersizeptr;

    if (PyArray_SIZE(self) == 0) {
        return 0;
    }

    iter = NpyIter_New(self, NPY_ITER_READONLY|
                             NPY_ITER_EXTERNAL_LOOP|
                             NPY_ITER_REFS_OK,
                        NPY_KEEPORDER, NPY_NO_CASTING,
                        NULL);
    if (iter == NULL) {
        return -1;
    }

    iternext = NpyIter_GetIterNext(iter, NULL);
    if (iternext == NULL) {
        NpyIter_Deallocate(iter);
        return -1;
    }
    dataptr = NpyIter_GetDataPtrArray(iter);
    strideptr = NpyIter_GetInnerStrideArray(iter);
    innersizeptr = NpyIter_GetInnerLoopSizePtr(iter);

    do {
        char* data = *dataptr;
        npy_intp stride = *strideptr;
        npy_intp count = *innersizeptr;

        while (count--) {
            if (nonzero(data, self)) {
                ++nonzero_count;
            }
            data += stride;
        }

    } while(iternext(iter));

    NpyIter_Deallocate(iter);

    return nonzero_count;
}
*/


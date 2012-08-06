import numpy
from ctypes import *
from numpy.ctypeslib import ndpointer

lib = cdll.LoadLibrary('./npyinterp.so')
"""
a = numpy.array([[0.1,0.2],[0.3,0.4]])
lib.trace2.restype = c_double
#lib.trace2.argtypes = [ndpointer(c_double)] #, ndpointer(c_double)]
lib.trace2.argtypes = [ndpointer(dtype=numpy.float64, ndim=2, flags='C_CONTIGUOUS'), c_int, c_int]
r = lib.trace2(a, a.shape[0], a.shape[1])
print r, 0.1+0.4
"""
lib.interpolate.argtypes = [
	ndpointer(dtype=numpy.float32, ndim=1, flags='C_CONTIGUOUS'), 
	ndpointer(dtype=numpy.float32, ndim=1, flags='C_CONTIGUOUS'), 
	ndpointer(dtype=numpy.float32, ndim=1, flags='C_CONTIGUOUS'), 
	c_int, 
	ndpointer(dtype=numpy.float64, ndim=1, flags='C_CONTIGUOUS'), 
	ndpointer(dtype=numpy.float64, ndim=1, flags='C_CONTIGUOUS'), 
	c_int]


x = numpy.array(range(10000), dtype=numpy.float64)*1.
y = numpy.array(range(10000), dtype=numpy.float64)*1.
r = numpy.array(range(10000 - 10), dtype=numpy.float32)*1. + 3.1
q = numpy.array(range(10000 - 10), dtype=numpy.float32)*1. + 6.121
z = numpy.zeros_like(r)

def run():
	return lib.interpolate(r, q, z, len(r), x, y, len(y))
def run2():
	return numpy.interp(q, x, y) - numpy.interp(r, x, y)
run()
print z, len(r), len(y)
print run2()



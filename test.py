import monointerp
import numpy

# data for interpolation
x = numpy.array(range(10000), dtype=numpy.float64)*1.
y = numpy.array(range(10000), dtype=numpy.float64)*1.

# lower and upper bin borders to integrate within
r = numpy.array(range(10000 - 10), dtype=numpy.float64)*1. + 3.1
q = numpy.array(range(10000 - 10), dtype=numpy.float64)*1. + 6.121

def run():
	# this is how to call the function, simple!
	return monointerp.interp(r, q, x, y)

def run2():
	# for verification, we compare to numpy.interp
	return numpy.interp(q, x, y) - numpy.interp(r, x, y)


def test():

	z = run()
	print(z, len(r), len(y))
	z_check = run2()
	print(z_check)
	assert numpy.allclose(z, z_check)
	

if __name__ == '__main__':
	test()

from jubatus.common import Datum
from jubatus.regression.client import Regression
import sys
import math

def main():
    cl = Regression("127.0.0.1", 9199, "test")
    
    d = Datum()
    for i in xrange(10):
        d.add_number('x', 1)
        d.add_number('y', 4)
        cl.train([[10.0, d]])

    d = Datum()
    d.add_number('x', 1)
    d.add_number('y', 4)
    result = cl.estimate([d])
    print("{0:30.30f}".format(result[0]))

if __name__ == "__main__":
    main()

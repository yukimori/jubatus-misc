# -*- coding:utf-8 -*-

from __future__ import print_function
import jubatus
from jubatus.common import Datum
from jubatus.classifier.types import LabeledDatum
from jubatus import Classifier

def main():
    # cl = jubatus.NearestNeighbor("localhost", 9199, "nn")
    cl = Classifier("localhost", 9199, "test")
    cl.clear()

    d = Datum()
    for i in xrange(10):
        d.add_number('x', 1)
        d.add_number('y', 4)
        cl.train([LabeledDatum("label1", d)])
        # cl.set_row(str(i), d)
        
    # scores = cl.similar_row_from_id(str(0), 10)
    # for score in scores:
    #     print("{0} {1:30.30f}".format(score.id, score.score))

    d = Datum()
    d.add_number('x', 1)
    d.add_number('y', 4)
    predict = cl.classify([d])
    print(predict)
    for score in predict:
        print("{0} {1:30.30f}".format(score[0].label, score[0].score))

if __name__ == "__main__":
    main()

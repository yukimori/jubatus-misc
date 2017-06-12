from jubatus.common import Datum
import jubatus

client = jubatus.Weight("127.0.0.1", 9199, "")

d = Datum()

d.add_number("user/age", 25)
d.add_number("user/income", 1000)
d.add_string("user/name", "Loren")
d.add_string("message", "Hello")

res = client.calc_weight(d)
print(res)

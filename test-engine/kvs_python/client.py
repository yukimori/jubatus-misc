# This file is auto-generated from kvs.idl with jenerator version /master
# *** DO NOT EDIT ***


import msgpackrpc
import jubatus.common
from .types import *
from jubatus.common.types import *

class Kvs(jubatus.common.ClientBase):
  def __init__(self, host, port, name, timeout=10):
    super(Kvs, self).__init__(host, port, name, timeout)

  def put(self, key, value):
    return self.jubatus_client.call("put", [key, value], TBool(), [TString(),
        TInt(True, 4)])

  def get(self, key):
    return self.jubatus_client.call("get", [key], TUserDef(Entry), [TString()])

  def get_average(self):
    return self.jubatus_client.call("get_average", [], TFloat(), [])

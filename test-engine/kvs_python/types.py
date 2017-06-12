# This file is auto-generated from kvs.idl with jenerator version /master
# *** DO NOT EDIT ***


import sys
import msgpack
import jubatus.common
from jubatus.common.types import *

class Entry:
  TYPE = TTuple(TString(), TInt(True, 4))

  def __init__(self, key, value):
    self.key = key
    self.value = value

  def to_msgpack(self):
    t = (self.key, self.value)
    return self.__class__.TYPE.to_msgpack(t)

  @classmethod
  def from_msgpack(cls, arg):
    val = cls.TYPE.from_msgpack(arg)
    return Entry(*val)

  def __repr__(self):
    gen = jubatus.common.MessageStringGenerator()
    gen.open("entry")
    gen.add("key", self.key)
    gen.add("value", self.value)
    gen.close()
    return gen.to_string()


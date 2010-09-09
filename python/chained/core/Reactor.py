from chained.ctypes import chained
from chained.core import connection,port
from ctypes import *

class Reactor (object):
	def __init__ (self):
		chained.so_chained.cis_init()
		chained.so_chained.cis_se_load_select()
		
	def run(self):
		chained.so_chained.cis_run()

	def listen(self, **kwargs):
		return port.Port(**kwargs)

	def connect(self, ip=None, port=None, protocol = None):
		try:
			if (ip == None) or (port == None) or protocol == None:
				return None
			c = chained.so_chained.cis_tcp_connect(ip, port)
			conn = connection.Connection(c, protocol)
			conn.protocol.connection = conn
			conn.protocol.onConnectionMade()
			return conn
		except:
			print "Fail"
		return None

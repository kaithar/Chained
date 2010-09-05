from chained.ctypes import chained
from chained.core import connection,protocol
from ctypes import *

class Port (protocol.Protocol):
	connection = None
	protocol = None

	def __init__(self, ip="0.0.0.0", port="0"):
		chained.so_chained.cis_tcp_listen.restype = POINTER(chained.Connection)
		chained.so_chained.cis_tcp_listen.argtypes = [c_char_p, c_int]
		c = chained.so_chained.cis_tcp_listen(ip, port)
		self.connection = connection.Connection(c,self)

	def onNewConnection(self, newcn):
		print self.connection, newcn
		nconn = connection.Connection(newcn, self.protocol())
		nconn.protocol.connection = nconn
		nconn.protocol.onConnectionMade()
		return 0
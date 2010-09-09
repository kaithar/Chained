from chained.ctypes import chained
from chained.core import connection,protocol
from ctypes import *

class Port (protocol.Protocol):
	connection = None
	protocol = None

	def __init__(self, ip="0.0.0.0", port=0, protocol = None):
		c = chained.so_chained.cis_tcp_listen(ip, port)
		self.connection = connection.Connection(c,self)
		self.protocol = protocol

	def onNewConnection(self, newcn):
		print "Port::onNewConnection ", self.connection, newcn, self.protocol
		nconn = connection.Connection(newcn, self.protocol())
		nconn.protocol.connection = nconn
		nconn.protocol.onConnectionMade()
		return 0
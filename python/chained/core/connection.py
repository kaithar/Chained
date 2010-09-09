from chained.ctypes import chained
from chained.core.buffer import Buffer

class Connection (object):
	ctype = None
	protocol = None
	recvq = None
	sendq = None

	def __init__(self, conn = None, protocol = None):
		print "Connection::__init__ ", conn, protocol
		if conn:
			self.ctype = conn
			conn.contents.callback_connected = chained.vc(self.callback_connected)
			conn.contents.callback_connect_failed = chained.vci(self.callback_connect_failed)
			conn.contents.callback_read = chained.ic(self.callback_read)
			conn.contents.callback_accept = chained.icc(self.callback_accept)
			conn.contents.callback_remote_lost = chained.ic(self.callback_remote_lost)
			conn.contents.callback_shutdown = chained.ic(self.callback_shutdown)
			conn.contents.callback_close = chained.ic(self.callback_close)
			self.recvq = Buffer(self.ctype.contents.recvq)
			self.sendq = Buffer(self.ctype.contents.sendq)
		if protocol:
			self.protocol = protocol

	def callback_connected(self, c):
		if self.protocol:
			return self.protocol.onConnectionMade() or 0
		return 0

	def callback_connect_failed(self, c, i):
		if self.protocol:
			return self.protocol.onConnectionFailed(i) or 0
		return 0

	def callback_read(self, c):
		if self.protocol:
			return self.protocol.onDataAvailable() or 0
		return 0

	def callback_accept(self, c, newconn):
		if self.protocol:
			return self.protocol.onNewConnection(newconn) or 0
		return 0

	def callback_remote_lost(self, c):
		if self.protocol:
			return self.protocol.onRemoteLost() or 0
		return 0

	def callback_shutdown(self, c, i):
		if self.protocol:
			return self.protocol.onShutdown(i) or 0
		return 0

	def callback_close(self, c):
		if self.protocol:
			return self.protocol.onClose() or 0
		return 0

	def write(self, s):
		chained.so_chained.cprintf(self.ctype, "%s", s)

	def close(self):
		self.ctype.contents.close(self.ctype)

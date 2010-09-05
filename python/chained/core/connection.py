from chained.ctypes import chained

class Connection (object):
	ctype = None
	protocol = None

	def __init__(self, conn = None, protocol = None):
		if conn:
			self.ctype = conn
			conn.contents.callback_connected = chained.vc(self.callback_connected)
			conn.contents.callback_connect_failed = chained.vci(self.callback_connect_failed)
			conn.contents.callback_read = chained.ic(self.callback_read)
			conn.contents.callback_accept = chained.icc(self.callback_accept)
			conn.contents.callback_remote_lost = chained.ic(self.callback_remote_lost)
			conn.contents.callback_shutdown = chained.ic(self.callback_shutdown)
			conn.contents.callback_close = chained.ic(self.callback_close)
		if protocol:
			self.protocol = protocol

	def callback_connected(self, c):
		if self.protocol:
			return self.protocol.onConnectionMade()
		return 0

	def callback_connect_failed(self, c, i):
		if self.protocol:
			return self.protocol.onConnectionFailed(i)
		return 0

	def callback_read(self, c):
		if self.protocol:
			return self.protocol.onDataAvailable()
		return 0

	def callback_accept(self, c, newconn):
		if self.protocol:
			return self.protocol.onNewConnection(newconn)
		return 0

	def callback_remote_lost(self, c):
		if self.protocol:
			return self.protocol.onRemoteLost()
		return 0

	def callback_shutdown(self, c, i):
		if self.protocol:
			return self.protocol.onShutdown(i)
		return 0

	def callback_close(self, c):
		if self.protocol:
			return self.protocol.onClose()
		return 0

	def write(self, s):
		chained.so_chained.cprintf(self.ctype, s)

	def close(self):
		ctype.close(self.ctype)

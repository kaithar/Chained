from chained.ctypes.chained import so_chained
from chained.core import port, protocol

so_chained.cis_init()
so_chained.cis_se_load_select()

class foo (protocol.Protocol):
	def onClose(self):
		print "Connection gone away!"
		return 0
		
	def onDataAvailable(self):
		s = self.connection.recvq.pop_to_delim() # Defaults to up to 10000 characters delimited by \n
		if (s[0] > 0):
			self.connection.write("You said: %s"%s[1])
		if (s[0] == -1):
			print "Eof without delim, discarding.\n"
			self.connection.recvq.empty()
		elif (s[0] == -2):
			self.connection.write("Neither EOF nor Delim\n")
		elif (s[0] == -3):
			self.connection.write("Line exceeds our ridiculous chunk size\n")
		return 0

p = port.Port(port=12345)
p.protocol = foo

so_chained.cis_run()


from chained.ctypes.chained import so_chained
from chained.core import port, protocol

so_chained.cis_init()
so_chained.cis_se_load_select()

class foo (protocol.Protocol):
	def onDataAvailable(self):
		self.connection.write("Rawr!")
		return 0

p = port.Port(port=12345)
p.protocol = foo

so_chained.cis_run()


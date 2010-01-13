import chained.primative.net as chained

class Port (object):
	# Stuff you should override...
	name = "Some port"
	protocol = None
	port = 0

	# Stuff you could override...
	ip = "0.0.0.0"

	# Stuff you should leave alone.
	connection = None
	connections = None
	
	def __init__(self):
		self.connections = []
		self.connection = chained.Port(self.name, self.ip, self.port, self)

	def accepted(self, cn):
		cn.protocol = self.protocol(cn, self)
		self.connections.append(cn.protocol)
		self.onAccept(cn)

	def onAccept(self, cn):
		# Override me!
		pass

	def childClosed(self, cn):
		self.connections.remove(cn)
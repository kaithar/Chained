from chained import run
import chained.net.Port
import chained.net.Protocol

conns = []


class testproto (chained.net.Protocol):
	def onRead (self, line):
		print self.connection.fd,">",line
		for c in conns:
			c.write("%d > %s\n"%(self.connection.fd, line))

	def onClose (self):
		print self.connection.fd,"-- Connection closed"
		for c in conns:
			c.write("%d -- Connection closed\n"%(self.connection.fd,self.connection.source))


class ConnectFactory (object):
	autoconnect = True
	reconnect = False

	name = "Connection"
	address = "127.0.0.1"
	port = 0
	protocol = None

	# Stuff you should leave alone.
	connection = None

	def __init__(self):
		if self.autoconnect:
			self.connect()

	def onConnect(self, cn):
		pass

	def onChildClosed(self, cn):
		pass

	def childClosed(self, cn):
		self.connection = None

	def reconnect(self):
		pass

	def connect(self):
		if self.connection:
			raise Exception("Factory already connected")
		cn = chained.connect(self.name, self.address, self.port)
                self.connection = self.protocol(cn, self)
                cn.protocol = self.connection

class testconnecter (ConnectFactory):
	name = "Test connect"
	address = "127.0.0.1"
	port = 10021
	protocol = testproto


class testport (chained.net.Port):
	name = "Test port"
	protocol = testproto
	port = 23568
	def onAccept(self, cn):
		print "New connection from %s"%(cn.source)
		for c in self.connections:
			c.write("%d -- New connection from %s\n"%(cn.fd,cn.source))
		cn.write("Hey there %s\n"%(cn.source))


f = testport()
try:
#	g = chained.connect("test conn", "fire.pounces.me", 6667)
	print `g`
	g.protocol = testproto()
except:
	pass
try:
	run()
except:
	pass

print "Meep"

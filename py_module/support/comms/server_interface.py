import chained.net as chained
from chained.comms.protocol import Protocol

class Factory (object):
	services = None
	connections = None
	def __init__ (self):
		self.connections = []
		self.services = {}
		
	def onAccept(self, p, cn):
		print "New connection from %s"%(cn.source)
		pr = Protocol()
		pr.transport = cn
		pr.factory = self
		cn.protocol = pr
		cn.write("GREETZ 10 %s\n"%("NOTRANDATALL"))
		pr.connectionMade()
		

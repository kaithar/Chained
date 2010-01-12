from chained import run
import chained.net.Port

conns = []

class testproto (object):
	def onRead (self, cn, line):
		print cn.fd,">",line
		for c in conns:
			c.write("%d > %s\n"%(cn.fd, line))
	def onClose (self, cn):
		conns.remove(cn)
		print `conns`
		print cn.fd,"-- Connection closed"
		for c in conns:
			c.write("%d -- Connection closed\n"%(cn.fd,cn.source))

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

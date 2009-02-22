import chained.cnet as chained

conns = []

class testport (object):
	def onAccept(self, p, cn):
		cn.protocol = testproto()
		conns.append(cn)
		print `conns`
		print "New connection from %s"%(cn.source)
		for c in conns:
			c.write("%d -- New connection from %s\n"%(cn.fd,cn.source))
		cn.write("Hey there %s\n"%(cn.source))

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

f = chained.Port("test port", "0.0.0.0", 23568, testport())
try:
#	g = chained.connect("test conn", "fire.pounces.me", 6667)
	print `g`
	g.protocol = testproto()
except:
	pass
try:
	chained.run()
except:
	pass

print "Meep"
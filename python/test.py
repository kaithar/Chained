from chained.core import protocol, reactor

class relay (protocol.Protocol):
	p = None
	def onDataAvailable(self):
		s = self.connection.recvq.pop_to_delim() # Defaults to up to 10000 characters delimited by \n
		if (s[0] > 0):
			print "relay got: %s"%s[1][:-1]
			self.p.connection.write(s[1])
		if (s[0] == -3):
			print "Line exceeds our ridiculous chunk size"
			self.connection.close()
			self.p.connection.close()
	
	def onClose(self):
		self.p.connection.close()

class foo (protocol.Protocol):
	s = None

	def onConnectionMade (self):
		r = relay()
		s = reactor.connect("irc.pounces.me", 6667, r)
		r.p = self
		self.s = r

	def onClose(self):
		self.s.connection.close
		
	def onDataAvailable(self):
		s = self.connection.recvq.pop_to_delim() # Defaults to up to 10000 characters delimited by \n
		if (s[0] > 0):
			print "foo got: %s"%s[1][:-1]
			self.s.connection.write(s[1])
		if (s[0] == -3):
			print "Line exceeds our ridiculous chunk size"
			self.connection.close()
			self.s.connection.close()

p = reactor.listen(port=12345, protocol = foo)
reactor.run()


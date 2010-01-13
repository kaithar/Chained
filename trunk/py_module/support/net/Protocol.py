class Protocol (object):
	connection = None
	port = None
	def __init__(self, cn, parent):
		self.connection = cn
		if parent:
			self.parent = parent

	def reading(self, cn, line):
			self.onRead(line)

	def onRead(self, line):
		# Override me
		pass

	def closing(self, cn):
			if self.parent:
				self.parent.connections.remove(self)
			self.onClose()

	def onClose(self):
		# Override me
		pass

        def write(self, line):
                self.connection.write(line)
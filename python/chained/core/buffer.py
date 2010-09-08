from chained.ctypes import chained
from ctypes import create_string_buffer

class Buffer (object):
	ctype = None

	def __init__(self, c = None):
		self.ctype = c

	#def store (self, block, size):
		# buffer_store (buffer_queue *buf, char *block, unsigned int size);

	#def eof(self):
		# extern void buffer_eof (buffer_queue *buf);

	def pop_by_size(self, size):
		# extern int buffer_pop_by_size (buffer_queue *buf, char *block, unsigned int size);
		s = create_string_buffer('\0' * size)
		status = chained.so_chained.buffer_pop_by_size(self.ctype, s, size)
		return (status, str(s.value))

	#/**
  # * The actual work of this is done in buffer_pop_by_size, this function just determines the size.
  # * @return Returns number of bytes writen to block.
  # * -1 means eof flag encountered before delim
  # * -2 means neither delim nor eof is present in the queue
  # * -3 means your buffer wasn't big enough to hold all data up to requested delim.
  # * Consider -1 fatal, -2 try again later, and -3 try again now with bigger buffer.
  # */
	def pop_to_delim(self, max_size = 10000, delim = '\n'):
		# extern int buffer_pop_to_delim (buffer_queue *buf, char *block, unsigned int size, char delim);
		size = 1000
		while True:
			s = create_string_buffer('\0' * size)
			status = chained.so_chained.buffer_pop_by_size(self.ctype, s, size, delim)
			if (status == -1) or (status == -2):
				return (status, None)
			elif (status == -3):
				size += 1000
				if (size > max_size):
					return (-3, None)
				continue
			else:
				break
		return (status, str(s.value))

	def peek(self):
		# extern char buffer_peek (buffer_queue *buf);
		return chained.so_chained.buffer_peek(self.ctype)

	def empty(self):
		# extern int buffer_empty (buffer_queue *buf);
		return (chained.so_chained.buffer_empty(self.ctypes) == 1)

	def free(self):
		# extern void buffer_free (buffer_queue *buf);
		chained.so_chained.buffer_free(self.ctypes)

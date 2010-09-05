from ctypes import *

chained = cdll.LoadLibrary("libchained.so")

#
# Forward Declarations
#

class connection(Structure):
	pass

class hook(Structure):
	pass

class event(Structure):
	pass

class timer_event(Structure):
	pass

#
# Function pointer structures
#

# void (*)(connection *);
vc = CFUNCTYPE(None, POINTER(connection))
# void (*)(connection *, int);
vci = CFUNCTYPE(None, POINTER(connection), c_int)
# int (*)(connection *);
ic = CFUNCTYPE(c_int, POINTER(connection))
# int (*)(connection *, int);
ici = CFUNCTYPE(c_int, POINTER(connection), c_int)
# int (*) (connection *, connection *);
icc = CFUNCTYPE(c_int, POINTER(connection), POINTER(connection))
#	int (*)(int version, int parc, void **parv);
iiiv = CFUNCTYPE(c_int, c_int, c_int, POINTER(c_void_p))
# int (*)(timer_event *, void *)
itv = CFUNCTYPE(c_int, POINTER(timer_event), c_void_p)

#
# Strutures!
#

class state_bitfield(Structure):
	_fields_ = [
		("connecting", c_int, 1),
		("reaping", c_int, 1),
		("remote_closed", c_int, 1),
		("local_read_shutdown", c_int, 1),
		("local_write_shutdown", c_int, 1),
		("can_shutdown", c_int, 1),
		("listen_read", c_int, 1),
		("listen_write", c_int, 1)
	]

connection._fields_ = [
		("fd", c_int),
		("ssl", c_void_p),
		("file", c_void_p),
		("source", c_char * 256),
		("data", c_void_p),
		("conn_mark", c_int),
		("syscall_error", c_int),
		("encapsulates", POINTER(connection)),
		("encapsulated_by", POINTER(connection)),
		("state", state_bitfield),
		("recvq", c_void_p), # Actually a buffer_queue, but that's irrelevant here.
		("sendq", c_void_p), # Ditto
		# Function pointers that we may be interested in.
		("connected", vc), # void (*connected)(connection *);
		("connect_failed", vci), # void (*connect_failed)(connection *, int);
		("write", ic), # int (*write)(connection *);
		("read", ic), # int (*read)(connection *);
		("shutdown", ici), # int (*shutdown)(connection *, int);
		("close", ic), # int (*close)(connection *);
		("callback_connected", vc), # void (*callback_connected)(connection *);
		("callback_connect_failed", vci), # void (*callback_connect_failed)(connection *, int);
		("callback_read", ic), # int (*callback_read) (connection *);
		("callback_accept", icc), # int (*callback_accept) (connection *on, connection *newcn);
		("callback_remote_lost", ic), # int (*callback_remote_lost) (connection *);
		("callback_shutdown", ic), # int (*callback_shutdown) (connection *, int);
		("callback_close", ic) # int (*callback_close)(connection *);
	]

hook._fields_ = [
		("event", c_ubyte * 101), # unsigned char event[101];
		("callback", iiiv) # int (*callback)(int version, int parc, void **parv);
]

event._fields_ = [
		("event", c_ubyte * 101), # unsigned char event[101];
		("references", c_uint), # unsigned int references;
		("hooks", c_void_p) # linklist_root *hooks; // we're casting void here since we really don't care about interacting with that directly.
]

timer_event._fields_ = [
		("prev", POINTER(timer_event)), #timer_event *prev, *next; /**< Prev/Next event */
		("next", POINTER(timer_event)),
		("when", c_uint), # unsigned int when;                     /**< When does this timer trigger? */
		("delay", c_int), # int delay;                             /**< Delay before retriggering */
	  ("repetitions", c_int), # int repetitions;                 /**< How many times to do this event */
		("callback_param", c_void_p), # void *callback_param;      /**< param for callback */
	  ("callback", itv) # int (*callback)(timer_event *, void*); /**< the callback for this event, void* is some sort of value meaningful to the callback */
]

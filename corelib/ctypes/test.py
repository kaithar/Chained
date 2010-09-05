from chained import *
chained.cis_init()
chained.cis_se_load_select()
chained.cis_tcp_listen.restype = POINTER(connection)
chained.cis_tcp_listen.argtypes = [c_char_p, c_int]
c = chained.cis_tcp_listen("0.0.0.0", 12345)

cb_accept = CFUNCTYPE(c_int, POINTER(connection), POINTER(connection))

def callback_accept(on, newcn):
	print on, newcn
	return 1

cb = cb_accept(callback_accept)
c.contents.callback_accept = cb
chained.cis_run()


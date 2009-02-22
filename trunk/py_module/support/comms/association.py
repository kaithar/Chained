from time import time

class Association:
        'This forms a link between the connection source and the service.'
        source = None
        service = None
        channelid = None
        closed = False

        def __init__(self, src, srv, cid):
                self.source = src
                self.service = srv
                self.channelid = cid

        def write(self, line):
                snd = "%s = %s"%(self.channelid, line)
                print ("%s Send > %s: %s"%(int(time()), self.source.fmtLineRecv, snd))
                return self.source.transport.write(snd+"\n")

        def close(self):
                if self.closed:
                        return
                self.closed = True
                snd = "%s !"%(self.channelid)
                print ("%s Send > %s: %s"%(int(time()), self.source.fmtLineRecv, snd))
                self.source.transport.write(snd + "\n")
                self.service.connectionLost(self)
                if (self.source.channels.has_key(self.channelid)):
                        del self.source.channels[self.channelid]

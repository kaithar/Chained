from time import time
from chained.comms.association import Association

class Protocol(object):
        # Various vars...
        user = None
        lastActivity = 0
        lastPing = 0
        lively = True
        fmtConnLost = "Unknown Connection dropped! ... %s"
        fmtLineRecv = "Unknown"

        def connectionMade(self):
                self.channels = {}
                self.lastActivity = time()
                self.factory.connections.append(self)

        def onClose(self, cn):
                for ch in self.channels.keys()[0:]:
                        self.channels[ch].service.connectionLost(self.channels[ch])
                self.user = None
                self.channels = {}
                self.factory.connections.remove(self)

        def onRead(self, cn, line):
                nowish = time()
                self.lastActivity = nowish
                words = line.split(" ")
                print "%s Recv < %s: %s"%(int(nowish), self.fmtLineRecv,`words`)

                if (len(words) >= 2):
                        # If there aren't 2 params (a channel id and a message type) we really don't wanna know :p
                        if (words[1] == "="):
                                # Continuation.  Simple to handle.
                                if (self.channels.has_key(words[0])):
                                        # Channel exists
                                        self.channels[words[0]].service.lineReceived(self.channels[words[0]], words[2:])
                                else:
                                        # We don't have a channel by this id.  Send a close back down the line just in case.
                                        snd = "%s !"%(words[0])
                                        print ("%s Send > %s: %s"%(int(time()), self.fmtLineRecv, snd))
                                        self.transport.write(snd + "\n")

                        elif (words[1] == "!"):
                                # Channel close.
                                if (self.channels.has_key(words[0])):
                                        # Channel exists
                                        self.channels[words[0]].service.connectionLost(self.channels[words[0]], words[2:])
                                        # We recheck, to make sure connectLost hasn't removed the channel from our list.
                                        if (self.channels.has_key(words[0])):
                                                del self.channels[words[0]]
                                else:
                                        # A close for a channel we don't have?  No point sending a close back.
                                        print ("%s FAIL ! %s: No channel to close!"%(int(time()), self.fmtLineRecv))
                                        pass

                        else:
                                # Either a channel open or an unsupported protocol token.
                                if (words[1] in self.factory.services) and not (self.channels.has_key(words[0])):
                                        assoc = Association(self, self.factory.services[words[1]], words[0])
                                        self.channels[words[0]] = assoc
                                        self.factory.services[words[1]].connectionMade(assoc)
                                        if (len(words) >= 3):
                                                self.factory.services[words[1]].lineReceived(assoc, words[2:])
                                else:
                                        # So we don't know what they're talking about, or we do but this would be a channel collision.
                                        # Either way, send back a close and don't go any further.
                                        snd = "%s !"(words[0])
                                        print ("%s Send > %s: %s"%(int(time()), self.fmtLineRecv, snd))
                                        self.transport.write(snd + "\n")

class Service:
        """This class forms the basis of a service that this comms server provides.
        A service class will be instanced once and all requests to that service will be made against that instance.
        See association.py for connection unique information."""

        db = None # Default behaviour is to assign a reference to globaldb here...

        def __init__(self, globaldb):
                'If you have a pressing need to implement something to happen when this service is instanced, do it here.'
                self.db = globaldb

        def connectionMade (self, association):
                'When the channel is initially created, this function is called to inform the service of the fact and allow any needed introductions to made.'
                pass

        def lineReceived (self, association, args):
                '''Called when a line is sent to the channel.
                The default behaviour is to try to call handleMeep (association, splitRestOfLine) where Meep is the first word in the line.
                Capitalisation case will be applied to the command, ensure you define handlers like "handleMeep".'''
                cmd = args[0].capitalize()
                if (hasattr(self, "handle"+cmd)):
                        getattr(self, "handle"+cmd)(association, args[1:])
                else:
                        getattr(self, "handleUnknown")(association, args[0], args[1:])

        def connectionLost (self, association, reason = None):
                '''When the channel is finally closed, this function is called to allow the service to do any clean up needed.
                This function will be called regardless of which side closes the connection.'''
                pass

        def handleUnknown (self, association, cmd, args):
                '''We've received a command that we don't know what to do with.'''
                pass

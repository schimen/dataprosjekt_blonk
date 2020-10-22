import asyncio

class Schocket:
    def __init__(self, reader, writer):
        self.reader = reader
        self.writer = writer

    async def send(self, message):
        self.writer.write(message.encode())

    async def read(self, max_size=1000):
        response = await self.reader.read(max_size)
        return response.decode()

    async def get_address(self):
        address = self.writer.get_extra_info('peername')
        return address

    async def close_connection(self):
        self.writer.close()

    def is_closing(self):
        return self.writer.is_closing()

    async def drain(self):
        await self.writer.drain()

class Client(Schocket):
    def __init__(self, ip, port, loop):
        self.ip = ip
        self.port = port
        self.loop = loop

    async def connect(self):
        reader, writer = await asyncio.open_connection(self.ip, self.port)
        self.reader = reader
        self.writer = writer

    async def listen(self, response_handler, *args, **kwargs):
        while not self.is_closing():
            response = await self.read()
            response_handler(response, *args, **kwargs)

def run_server_forever(host, port, handler):
    """
    runs server in event loop
    """
    loop = asyncio.get_event_loop()
    task = asyncio.start_server(handler, host, port, loop=loop)
    server = loop.run_until_complete(task)
    try:
        loop.run_forever()

    except KeyboardInterrupt:
        server.close()
        loop.run_until_complete(server.wait_closed())
        loop.close()

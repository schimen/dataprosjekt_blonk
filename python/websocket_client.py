import asyncio
import websockets

class Client:
    async def connect(self, uri):
        self.queue = asyncio.Queue()
        async with websockets.connect(uri) as websocket:
            print(f'connected to {uri}')
            self.websocket = websocket
            while True:
                try:
                    response = await self.websocket.recv()
                    print(response)
                    #await self.queue.put(response)

                except websockets.ConnectionClosed:
                    print('connection closed')
                    break

    async def send(self, command, *arguments):
        message = ';'.join((command, *arguments)) + r'\n'
        await self.websocket.send(message)

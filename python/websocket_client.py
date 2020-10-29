import asyncio
import websockets

class Client:
    async def connect(self, uri):
        self.queue = asyncio.Queue()
        async with websockets.connect(uri) as websocket:
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

if __name__ == '__main__':
    uri = "ws://localhost:8888"
    client = Client()
    loop = asyncio.get_event_loop()
    loop.run_until_complete(client.connect(uri))

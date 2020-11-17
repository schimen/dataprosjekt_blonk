"""
bibliotek for websocket klient i prosjekt i faget IELET2001
klienten kobler seg på vår egen skrevet server og bruker denne vår egen
protokoll for kommandoer
"""

import asyncio
import websockets

class Client:
    """
    klient klasse
    """
    def __init__(self, host, port, message_handler=print):
        self.host = host
        self.port = port
        self.message_handler = message_handler

    async def connect(self, uri):
        """
        koble til sever og lagre nødvendig informasjon
        """
        async with websockets.connect(uri) as websocket:
            self.websocket = websocket
            print(f'connected to {uri}')
            while True:
                try:
                    response = await self.websocket.recv()
                    self.message_handler(response)

                except websockets.ConnectionClosed:
                    print('connection closed')
                    break

    def send_soon(self, *arguments):
        asyncio.create_task(self.send(*arguments))

    async def send(self, *arguments):
        """
        formaterer melding i riktig protokoll.
        """
        message = ';'.join((*arguments,)) + '\n'
        await self.websocket.send(message)

    async def start(self):
        uri = f"ws://{self.host}:{self.port}"
        asyncio.create_task(self.connect(uri))

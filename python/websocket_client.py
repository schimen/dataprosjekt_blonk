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
    async def connect(self, uri):
        """
        koble til sever og lagre nødvendig informasjon
        """
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
        """
        formaterer melding i riktig protokoll.
        """
        message = ';'.join((command, *arguments)) + '\n'
        await self.websocket.send(message)

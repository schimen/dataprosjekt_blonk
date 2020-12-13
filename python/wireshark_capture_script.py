import asyncio
import websockets
from libraries.client_library import Client

async def main():
    uri = f"ws://{client.host}:{client.port}"
    async with websockets.connect(uri) as websocket:
        client.websocket = websocket
        await client.send('lytt', 'A1', 'START')
        await client.send('send', 'A1', 'dette er en melding')
        await client.send('send', 'A1', 'dette er en ny melding')
        print('done')

if __name__ == '__main__':
    host = input('host: ')
    port = input('port: ')
    client = Client(host, port)
    input('trykk enter når du er klar...')
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())

import asyncio
import websockets


async def main():
    uri = "ws://localhost:8888"
    websocket = await websockets.connect(uri)
    #asyncio.create_task(receive(websocket))
    while True:
        message = input('new message: ')
        await websocket.send(message)
        print(f'sent: {message}')
        #må vist ha dette for å funke?
        response = await websocket.recv()
        print(f'response: {response}')

async def send(websocket):
    while True:
        message = input()
        await websocket.send(message)
        print(f'sent {message}')

async def receive(websocket):
    while True:
        response = await websocket.recv()
        print(response)

asyncio.get_event_loop().run_until_complete(main())
asyncio.run_forever()

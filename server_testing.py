import asyncio
from schimens_schocket_library import Schocket, run_server_forever

async def echo(reader, writer):
    """
    keeps the connection with one socket til the connection is closed
    or the keyword is received.
    upon message from client the server calculates an answer and sends it
    """
    socket = Schocket(reader, writer)
    while True:
        message = await socket.read()
        print(f'received: {message}')
        response = message
        await socket.send(response)
        print(f'sent: {response}')

def main():
    """
    run the server forever (til manually interrupted) on host and port
    """
    host, port = ('127.0.0.1', 8888)
    run_server_forever(host, port, echo)

if __name__ == '__main__':
    main()

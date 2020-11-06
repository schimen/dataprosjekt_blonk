"""
todo:
    fiks parse message (den kutter ut små n-er på slutten)
"""

import socket
import asyncio
import websockets
from string import ascii_uppercase

class Connection:
    connections = []
    messages = {f'{letter}{i}': [] for letter in ascii_uppercase[:10] for i in range(10)}

    def __init__(self, websocket):
        self.id = self.get_id_by_websocket(websocket)
        self.listen = set()
        self.websocket = websocket
        self.connections.append(self)

    def remove(self):
        self.connections.remove(self)

    async def handle_message(self, message):
        command, arguments = parse_message(message)
        commands = {'send': self.send_handler,
                    'lytt': self.listen_handler,
                    'hent': self.get_last_handler,
                    'print': self.print_handler}

        if command not in commands:
            await self.websocket.send(f'feil;{command}\n')

        else:
            handler = commands[command]
            await handler(*arguments)

    async def send_handler(self, address, message, *args):
        """
        send;adress;message\n
        """
        print(f'{self.id} sent {message} to {address}')
        try:
            self.messages[address].append(message)

        except KeyError:
            self.messages[address] = []
            self.messages[address].append(message)

        for connection in filter(lambda x: address in x.listen, self.connections):
            print(f'server sent {message} to {connection.id}')
            await connection.websocket.send(f'melding;{address};{message}\n')

    async def listen_handler(self, address, action, *args):
        """
        listen;address;action\n
        """
        if action.lower() == 'start':
            self.listen.add(address)
            print(f'{self.id} listen to: {self.listen}')

        elif action.lower() == 'stop':
            self.listen.remove(address)
            print(f'{self.id} listen to: {self.listen}')

        else:
            print(f'{action} is no action')
            await self.websocket.send('feil;lytt\n')

        await self.websocket.send('ok\n')

    async def get_last_handler(self, address, *args):
        """
        get_last;address\n
        """
        try:
            last_message = self.messages[address][-1]

        except KeyError:
            last_message = ''

        await self.websocket.send(f'melding;{address};{last_message}\n')

    async def print_handler(self, *args):
        print('connections:')
        for connection in connectinos:
            print(connection.id)

        print()
        print('messages:')
        for address, message in messages.items():
            print(f'{address}: {message}')

    @staticmethod
    def get_id_by_websocket(websocket):
        remote_host, remote_port = websocket.remote_address[:2]
        return f'{remote_host}:{remote_port}'

    @staticmethod
    def get_connection_by_id(id):
        for connection in connections:
            if connection.id == id:
                return connection

    @staticmethod
    def get_connection_by_websocket(websocket):
        for connection in connections:
            if connection.websocket == websocket:
                return connection

def parse_message(message):
    """
    command;arguments\n
    """
    #todo: kutter ut små n på slutten av meldinger. fiks dette!
    stripped_message = message.strip(r'\n')
    parts = stripped_message.split(';')
    command = parts[0]
    try:
        arguments = parts[1:]

    except IndexError:
        arguments = []

    return command, arguments

async def connection_handler(websocket, path):
    connection = Connection(websocket)
    id = connection.id
    print(f'new connection from {id}')
    try:
        async for message in websocket:
            await connection.handle_message(message)

    except websockets.ConnectionClosed:
        print(f"{id}: connection closed")
        connection.remove()
        del connection

def get_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('10.255.255.255', 1))
        ip = s.getsockname()[0]

    except Exception:
        ip = '127.0.0.1'

    finally:
        s.close()

    return ip

if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    host = get_ip()
    port = 8000
    start_server = websockets.serve(connection_handler, host=host, port=port)
    print(f'hosting server at "{host}:{port}"')
    loop.run_until_complete(start_server)
    loop.run_forever()

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
                    'connect': self.connect_handler,
                    'disconnect': self.disconnect_handler}

        if command not in commands:
            await self.websocket.send(f'feil;{command}\n')

        else:
            handler = commands[command]
            await handler(*arguments)

    async def send_handler(self, address, message, *args):
        """
        send;adress;message\n
        """
        self.messages[address].append(message)
        for connection in filter(lambda x: address in x.listen, self.connections):
            print(f'send to: {connection.id}')
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
        last_message = self.messages[address][-1]
        await self.websocket.send(f'melding;{address};{last_message}\n')

    async def connect_handler(self, *args):
        """
        connect\n
        """
        await asyncio.sleep(0.5)
        print(f'connecting {self.id}')

    async def disconnect_handler(self, *args):
        """
        disconnect\n
        """
        await asyncio.sleep(0.5)
        print(f'disconnecting {self.id}')

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
    stripped_message = message.strip(r'\n')
    parts = stripped_message.split(';')
    command = parts[0]
    arguments = parts[1:]
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
    hostname = socket.gethostname()
    ip_address = socket.gethostbyname(hostname)
    return ip_address

if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    host = get_ip()
    port = 8000
    start_server = websockets.serve(connection_handler, host=host, port=port)
    print(f'hoting server at "{host}:{port}"')
    loop.run_until_complete(start_server)
    loop.run_forever()

"""
server script for dataprosjekt i IELET2001.
kjør server på raspbery pi og koble på med websockets.
se https://github.com/schimen/dataprosjekt_blonk for mer info
"""

import socket
import asyncio
import websockets
from database_control import Database

class Connection:
    """
    connection class
    used for handling all action regarding connections and their messages
    """
    connections = []
    database = Database()

    def __init__(self, websocket):
        """
        initialize connection object
        """
        self.connection_id = self.get_id_by_websocket(websocket)
        self.listen = set()
        self.websocket = websocket
        self.connections.append(self)

    def remove(self):
        """
        removes connection from connection list
        """
        self.connections.remove(self)

    async def handle_message(self, message):
        """
        handle each received message.
        this function decodes the message and calls the appropriate handler
        """
        command, arguments = parse_message(message)
        commands = {'send': self.send_handler,
                    'lytt': self.listen_handler,
                    'hent': self.get_last_handler}

        if command not in commands:
            await self.websocket.send(f'feil;{command}\n')

        else:
            handler = commands[command]
            try:
                await handler(*arguments)

            except TypeError:
                print('missing arguments')
                await self.websocket.send(f'feil;{command}\n')

    async def send_handler(self, address, message):
        """
        send;adress;message\n
        """
        print(f'{self.connection_id} sent {message} to {address}')
        asyncio.create_task(self.database.save_message(address, message, self.connection_id))

        for connection in filter(lambda x: address in x.listen, self.connections):
            print(f'server sent {message} to {connection.connection_id}')
            await connection.websocket.send(f'melding;{address};{message}\n')

    async def listen_handler(self, address, action):
        """
        lytt;address;action\n
        """
        if action.lower() == 'start':
            self.listen.add(address)
            print(f'{self.connection_id} listen to: {self.listen}')

        elif action.lower() == 'stop':
            self.listen.remove(address)
            print(f'{self.connection_id} listen to: {self.listen}')

        else:
            print(f'{action} is no action')
            await self.websocket.send('feil;lytt\n')

        await self.websocket.send('ok\n')

    async def get_last_handler(self, address):
        """
        hent;address\n
        """
        last_message = await self.database.get_last_message(address)
        await self.websocket.send(f'melding;{address};{last_message}\n')

    @staticmethod
    def get_id_by_websocket(websocket):
        """
        static method that returns an id consisting of ip_address and port
        """
        print(websocket.remote_address)
        remote_host, remote_port = websocket.remote_address[:2]
        return f'{remote_host}:{remote_port}'

def parse_message(message):
    """
    command;arguments\n
    """
    stripped_message = message.strip('\n')
    parts = stripped_message.split(';')
    command = parts[0]
    try:
        arguments = parts[1:]

    except IndexError:
        arguments = []

    return command, arguments

async def connection_handler(websocket, *args):
    """
    handles connection. called for each new connection
    """
    connection = Connection(websocket)
    connection_id = connection.connection_id
    print(f'new connection from {connection_id}')
    try:
        async for message in websocket:
            await connection.handle_message(message)

    except websockets.ConnectionClosed:
        print(f"{connection_id}: connection closed")
        connection.remove()
        del connection

def get_ip():
    """
    returns the ip of the computer.
    returns local ip if not connected to internet
    """
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        sock.connect(('10.255.255.255', 1))
        sock_ip = sock.getsockname()[0]

    except IndexError:   #finn ut hvilken error dette egentlig er
        sock_ip = '127.0.0.1'

    finally:
        sock.close()

    return sock_ip

def main():
    """
    main function, run all code here
    """
    loop = asyncio.get_event_loop()
    host = get_ip()
    port = 8000
    start_server = websockets.serve(connection_handler, host=host, port=port)
    print(f'hosting websockets server at "{host}:{port}"')
    try:
        loop.run_until_complete(start_server)
        loop.run_forever()

    except KeyboardInterrupt:
        print(f'terminated from user')
        loop.close()

if __name__ == '__main__':
    main()

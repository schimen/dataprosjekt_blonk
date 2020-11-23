"""
server script for dataprosjekt i IELET2001.
kjør server på raspbery pi og koble på med websockets.
se https://github.com/schimen/dataprosjekt_blonk for mer info

Author: Simen Løcka Eine
"""

import socket
import asyncio
import websockets
from libraries.database_library import Database

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
        Remove connection from conncetions list
        """
        self.connections.remove(self)

    async def handle_message(self, message):
        """
        Handle a received message.
        Parses message and calls appropriate function

        Parameters:
        message (str): message to proccess
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
        Handle "send" command

        Parameters:
        address (str): address to send message to
        message (str): message to send
        """
        asyncio.create_task(self.database.save_message(address, message, self.connection_id))

        for connection in filter(lambda x: address in x.listen, self.connections):
            await connection.websocket.send(f'melding;{address};{message}\n')

    async def listen_handler(self, address, action):
        """
        Handle "lytt" command

        Parameters:
        address (str): address to begin listening to
        action (str): choice of action for listening (start/stop)
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

    async def get_last_handler(self, address):
        """
        Get last message on database on specific address
        and send it to connection

        Parameters:
        address (str): address to get the last message from
        """
        last_message = await self.database.get_last_message(address)
        await self.websocket.send(f'melding;{address};{last_message}\n')

    @staticmethod
    def get_id_by_websocket(websocket):
        """
        Returns a combined string of ip address and port of connection
        """
        remote_host, remote_port = websocket.remote_address[:2]
        return f'{remote_host}:{remote_port}'

def parse_message(message):
    """
    Split message on ";" and strip message of "\n"

    Parameters:
    message (str): message to split

    Returns:
    str:command
    list:arguments
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
    Handle opening, traffic and closing new connection

    Parameters:
    websocket (WebSocketClientProtocol): Class from websockets library
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
    Return computer IP address
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
    Setup loop and run server forever
    """
    loop = asyncio.get_event_loop()
    host = get_ip()
    port = 8000
    start_server = websockets.serve(connection_handler, host=host, port=port,
                                    max_queue=10)
    print(f'hosting websockets server at "{host}:{port}"')
    try:
        loop.run_until_complete(start_server)
        loop.run_forever()

    except KeyboardInterrupt:
        print(f'terminated from user')
        loop.close()

if __name__ == '__main__':
    main()

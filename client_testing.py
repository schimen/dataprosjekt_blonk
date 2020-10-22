import asyncio
from schimens_socket_library import Client



def main():
    """
    function for running client
    """
    host, port = ('127.0.0.1', 8888)
    loop = asyncio.get_event_loop()
    client = Chat_client(host, port, loop)
    loop.create_task(client.receive_forever(max_size=100))
    loop.run_forever()

if __name__ == '__main__':
    main()

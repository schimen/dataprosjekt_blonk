import asyncio
from random import randint
from libraries.gui_library import GUI
from libraries.client_library import Client, parse_message

def send_lytt():
    print('sender lytt')
    address = 'potVal'
    client.send_soon('lytt', address, 'start')

def on_message(response):
    print(response)
    command, arguments = parse_message(response)
    string_value = arguments[1]
    try:
        value = int(string_value)
        plot.add_value('pot', value)

    except ValueError:
        pass

if __name__ == '__main__':
    #set up websocket client
    host = input('host: ')
    port = input('port: ')
    client = Client(host, port, message_handler=on_message)
    client

    section_content = ({'plot': {}, 'name': 'plot_test'},
                       {'button': {'text': 'lytt','command':send_lytt}})
    gui = GUI()
    section = gui.add_section(section_content)
    plot = section.callable_widgets['plot_test']
    plot.add_line('pot', color='g', style='-')
    loop = asyncio.get_event_loop()
    loop.run_until_complete(gui.start())
    loop.run_until_complete(client.start())
    try:
        loop.run_forever()

    except KeyboardInterrupt:
        print('terminated by user')

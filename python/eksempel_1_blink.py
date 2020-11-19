"""
Blink eksempel i eksempel 1:

Denne koden lager et vindu som kan sende beskjed om å skru av eller på led
"""

import asyncio
from libraries.gui_library import GUI
from libraries.client_library import Client
from tkinter import DISABLED

def led_on():
    message = 'send;led;on\n'
    client.send_soon(message)
    section.callable_widgets['led_indicator'].select()

def led_off():
    message = 'send;led;off\n'
    client.send_soon(message)
    section.callable_widgets['led_indicator'].deselect()

content = ({'label': {'text': 'Turn Led on/off'}},
           {'button': {'text': 'on', 'command': led_on}},
           {'button': {'text':'off', 'command': led_off}},
           {'checkbutton':{'state': DISABLED, 'text':'led state'},
            'name': 'led_indicator'})

if __name__ == '__main__':
    #set up websocket client
    host = input('host: ')
    port = input('port: ')
    client = Client(host, port)

    #set up gui:
    gui = GUI()
    section = gui.add_section(content)

    #run event loop:
    loop = asyncio.get_event_loop()
    loop.run_until_complete(gui.start())
    loop.run_until_complete(client.start())
    try:
        loop.run_forever()

    except KeyboardInterrupt:
        print('terminated by user')

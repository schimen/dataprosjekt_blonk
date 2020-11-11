import asyncio
from gui_library import GUI
from websocket_client import Client

def send_message():
    entry = section.callable_widgets['text_entry']
    message = entry.get()
    message = message.split(' ')
    client.send_soon(*message)

def on_message(response):
    label = section.callable_widgets['text_output']
    label.config(text=response)

section_content = ({'label': {'text': 'skill kommandoer med mellomrom'}},
                   {'entry': {}, 'name': 'text_entry'},
                   {'button': {'text': 'send', 'command': send_message}},
                   {'label': {'text': 'siste svar:'}},
                   {'label': {'font':'Arial 14 bold'}, 'name': 'text_output'})


if __name__ == '__main__':
    host = input('host: ')
    port = input('port: ')
    client = Client(host, port, message_handler=on_message)
    gui = GUI()
    section = gui.add_section(section_content)
    loop = asyncio.get_event_loop()
    loop.run_until_complete(gui.start())
    loop.run_until_complete(client.start())

    try:
        loop.run_forever()

    except KeyboardInterrupt:
        print('terminated by user')

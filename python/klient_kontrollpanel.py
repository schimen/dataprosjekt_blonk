"""
Dette er et enkelt kontrollpanel som kan brukes til å sende meldinger
til serveren og se de siste mottatte meldingene.
Dette kontrollpanalet har blitt brukt for å sjekke at kommandoene
på serveren fungerer som de skal.
"""

import asyncio
from libraries.gui_library import GUI
from libraries.client_library import Client
from libraries.database_library import Database

def send_message():
    entry = server_section.callable_widgets['text_entry']
    message = entry.get()
    message = message.split(';')
    client.send_soon(*message)

def on_message(response):
    label = server_section.callable_widgets['text_output']
    label.config(text=response)

def get_messages_handler():
    entry = db_section.callable_widgets['address_entry']
    address = entry.get()
    asyncio.create_task(show_messages(address))

async def show_messages(address):
    listbox = db_section.callable_widgets['message_listbox']
    n_messages = 10
    counter = 0

    #reset listbox
    for i in range(n_messages):
        listbox.insert(i, '')

    async for document in database.get_messages(query={'address': address}, max_length=n_messages):
        message = document['message']
        listbox.insert(counter, message)
        counter += 1


server_content = ({'label': {'text': 'skill kommandoer med semikolon (;)'}},
                  {'entry': {}, 'name': 'text_entry'},
                  {'button': {'text': 'send', 'command': send_message}},
                  {'label': {'text': 'siste svar:'}},
                  {'label': {'font':'Arial 14 bold'}, 'name': 'text_output'})

db_content = ({'button': {'text': 'hent siste meldinger',
               'command': get_messages_handler}},
              {'label': {'text': 'addresse:'}},
              {'entry': {}, 'name': 'address_entry'},
              {'listbox': {}, 'name': 'message_listbox'})

if __name__ == '__main__':
    #set up websocket client
    host = input('host: ')
    port = input('port: ')
    client = Client(host, port, message_handler=on_message)

    #connect to database (takes some time)
    database = Database()

    #set up gui:
    gui = GUI()
    server_section = gui.add_section(server_content)
    db_section = gui.add_section(db_content, row=0, column=1)

    #run event loop:
    loop = asyncio.get_event_loop()
    loop.run_until_complete(gui.start())
    loop.run_until_complete(client.start())
    try:
        loop.run_forever()

    except KeyboardInterrupt:
        print('terminated by user')

"""
Dette er en enkel demonstrasjon av kontrollpanel-biblioteket.
Dette eksempelet viser kun frem GUI, ikke nettverkskommunikasjon.
"""

from libraries.gui_library import GUI
import asyncio

def update_message():
    #denne funksjonen kalles ved klikk på knappen
    
    #her hentes teksten i 'text_entry'  feltet:
    entry = section.callable_widgets['text_entry']
    world = entry.get()

    message =  f'hei {world}'

    #her skrives den nye meldingen i 'text_output' feltet:
    label = section.callable_widgets['text_output']
    label.config(text=message)

section_content = ({'label': {'text': 'skriv navnet ditt'}},
                   {'entry': {}, 'name': 'text_entry'},
                   {'button': {'text': 'si hei',
                               'command': update_message}},
                   {'label': {}, 'name': 'text_output'})

#her settes kontrollpanelet opp:
gui = GUI()
section = gui.add_section(section_content)
loop = asyncio.get_event_loop()
loop.run_until_complete(gui.start())

#her kjøres kontrollpanelet:
try:
    loop.run_forever()

except KeyboardInterrupt:
    print('terminated by user')

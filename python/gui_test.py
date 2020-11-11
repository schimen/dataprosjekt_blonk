from gui_library import GUI
import asyncio

def update_message():
    entry = section.callable_widgets['text_entry']
    world = entry.get()
    message =  f'hei {world}'
    label = section.callable_widgets['text_output']
    label.config(text=message)

section_content = ({'label': {'text': 'skriv navnet ditt'}},
                   {'entry': {}, 'name': 'text_entry'},
                   {'button': {'text': 'si hei', 'command': update_message}},
                   {'label': {}, 'name': 'text_output'})

gui = GUI()
section = gui.add_section(section_content)
loop = asyncio.get_event_loop()
loop.run_until_complete(gui.start())
try:
    loop.run_forever()

except KeyboardInterrupt:
    print('terminated by user')

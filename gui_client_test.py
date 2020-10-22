import tkinter as tk
import asyncio
from schimens_schocket_library import Client

class Menubar(tk.Menu):
    def __init__(self, parent, *args, **kwargs):
        tk.Menu.__init__(self, parent, *args, **kwargs)
        self.parent = parent
        self.load_widgets()

    def load_widgets(self):
        file_menu = tk.Menu(self)
        file_menu.add_command(label="New")
        file_menu.add_command(label="Save")

        edit_menu = tk.Menu(self)
        edit_menu.add_command(label="test 1")
        edit_menu.add_command(label="test 2")

        self.add_cascade(label="File", menu=file_menu)
        self.add_cascade(label="Edit", menu=edit_menu)
        self.parent.config(menu=self)

class Section(tk.Frame):
    def __init__(self, parent, *args, **kwargs):
        tk.Frame.__init__(self, parent, *args, **kwargs)
        self.parent = parent

    def load_widgets(self, content):
        for widget in content:
            position = {}
            for key, preferences in widget.items():
                if key == 'button':
                    new_widget = tk.Button(self, preferences)

                elif key == 'scale':
                    new_widget = tk.Scale(self, preferences)

                else:
                    position[key] = preferences

            try:
                new_widget.grid(position)

            except UnboundLocalError:
                pass #vurder å legge inn noe her

def configure_root():
    root = tk.Tk()
    root.rowconfigure(0, weight=1)
    root.columnconfigure(0, weight=1)
    return root

async def create_async_gui(client, settings_content=({},), interval=0.05):
    root = configure_root()
    menubar = Menubar(root)

    tools = Section(root)
    settings = Section(root)

    tools_content = ({'button':{'text':'send "test"','command':lambda:asyncio.create_task(client.send('schimens test'))},'row':0,'column':0},
                     {'button':{'text':'connect','command':lambda:asyncio.create_task(startup_client(client))},'row':1,'column':0})
    tools.load_widgets(tools_content)
    settings.load_widgets(settings_content)

    tools.grid(row=0, column=0)
    settings.grid(row=0, column=1)
    while True:
        root.update()
        await asyncio.sleep(interval)

async def startup_client(client):
    await client.connect()
    address = await client.get_address()
    print(f'connected to {address}')
    asyncio.create_task(client.listen(lambda x: print(f'received: {x}')))

async def main(loop):
    host, port = ('127.0.0.1', 8888)
    client = Client(host, port, loop)
    gui_task = asyncio.create_task(create_async_gui(client))

if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main(loop))
    loop.run_forever()

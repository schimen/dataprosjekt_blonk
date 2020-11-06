import tkinter as tk
import asyncio
from websocket_client import Client

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

async def create_async_gui(client, interval=0.05):
    root = configure_root()
    tools = Section(root)
    tools_content = ({'button':{'text':'lytt A1','command':lambda:asyncio.create_task(client.send('lytt', 'A1', 'start'))},'row':0,'column':0},
                     {'button':{'text':'send test','command':lambda:asyncio.create_task(client.send('send', 'A1', 'test'))},'row':1,'column':0},
                     {'button':{'text':'hent A1','command':lambda:asyncio.create_task(client.send('hent', 'A1'))},'row':2,'column':0})
    tools.load_widgets(tools_content)
    tools.grid(row=0, column=0)

    while True:
        root.update()
        await asyncio.sleep(interval)

async def main(loop, ip, port):
    uri = f"ws://{ip}:{port}"
    client = Client()
    asyncio.create_task(client.connect(uri))
    asyncio.create_task(create_async_gui(client))

if __name__ == '__main__':
    try:
        ip = input('ip: ')
        port = input('port: ')
        loop = asyncio.get_event_loop()
        loop.run_until_complete(main(loop, ip, port))
        loop.run_forever()

    except KeyboardInterrupt:
        print('terminated from user')

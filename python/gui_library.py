import tkinter as tk
import asyncio

class GUI(tk.Tk):
    """
    klasse for å gi høynivå kontroll på gui
    """
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        self.rowconfigure(0, weight=1)
        self.columnconfigure(0, weight=1)

    def add_section(self,  content, **kwargs):
        section = Section(self)
        section.load_widgets(content)
        section.grid(kwargs)
        return section

    async def start(self):
        asyncio.create_task(self.async_loop())

    async def async_loop(self, interval=0.05):
        while True:
            try:
                self.update()
                await asyncio.sleep(interval)

            except KeyboardInterrupt:
                print('gui terminated from user')
                break

            except tk.TclError:
                print('window closed')
                break


class Section(tk.Frame):
    def __init__(self, parent, *args, **kwargs):
        tk.Frame.__init__(self, parent, *args, **kwargs)
        self.parent = parent
        self.callable_widgets = dict()

    def load_widgets(self, content):
        for widget in content:
            position = {}
            widget_handlers = {'button': tk.Button,
                               'scale': tk.Scale,
                               'entry': tk.Entry,
                               'label': tk.Label,
                               'listbox': tk.Listbox}

            for key, preferences in widget.items():
                if key in widget_handlers:
                    new_widget = widget_handlers[key](self, preferences)

                elif key in ('row', 'column'):
                    position[key] = preferences

                elif key == 'name':
                    self.callable_widgets[preferences] = new_widget

            try:
                new_widget.grid(position)

            except UnboundLocalError:
                pass #vurder å legge inn noe her

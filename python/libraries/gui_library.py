import asyncio
import tkinter as tk
from matplotlib import pyplot as plt
from matplotlib import style
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg


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
                               'listbox': tk.Listbox,
                               'checkbutton': tk.Checkbutton,
                               'plot': Plot}

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


class Plot(tk.Frame):
    def __init__(self, parent, *args, **kwargs):
        tk.Frame.__init__(self, parent, *args, **kwargs)
        self.parent = parent

        #settings
        plotstyle='ggplot'
        figsize=(5,5)
        dpi=100
        ylim=(0,100)

        self.width=100
        self.lines = dict()

        style.use(plotstyle)
        self.fig = plt.figure(figsize=figsize, dpi=dpi)
        self.ax = self.fig.add_subplot(1, 1, 1)
        self.ax.set_ylim(*ylim)
        self.last_x = self.width
        self.ax.set_xlim((0, self.last_x))

        self.canvas = FigureCanvasTkAgg(self.fig, parent)
        self.canvas.get_tk_widget().grid(column=1, row=1)

    def add_value(self, name, value):
        x_values, y_values = self.lines[name]['values']
        y_values.append(value)
        try:
            new_x = x_values[-1] + 1

        except IndexError:
            new_x = 0

        x_values.append(new_x)
        if self.last_x < len(x_values):
            self.last_x = len(x_values)

        if self.width < self.last_x:
            self.ax.set_xlim((self.last_x-self.width), self.last_x)

        color = self.lines[name]['color']
        style = self.lines[name]['style']
        self.ax.plot(x_values, y_values, 'r', label=name, color=color,
                     linestyle=style)
        self.canvas.draw()

    def add_line(self, name, color='r', style='-'):
        self.lines[name] = {'values': ([],[]), 'color': color, 'style': style}

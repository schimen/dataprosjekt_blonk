import asyncio
from random import randint
from libraries.gui_library import GUI

async def update_plot(name):
    while True:
        plot.add_value(name, randint(40,100))
        await asyncio.sleep(randint(1, 100)/1000)

async def main():
    test_1 = asyncio.create_task(update_plot('test_1'))
    test_2 = asyncio.create_task(update_plot('test_2'))
    await asyncio.gather(test_1, test_2)

section_content = ({'plot': {}, 'name': 'plot_test'},)
gui = GUI()
section = gui.add_section(section_content)
plot = section.callable_widgets['plot_test']
plot.add_line('test_1', color='g', style=':')
plot.add_line('test_2', color='b', style='-')
loop = asyncio.get_event_loop()
loop.run_until_complete(gui.start())
loop.run_until_complete(main())
try:
    loop.run_forever()

except KeyboardInterrupt:
    print('terminated by user')

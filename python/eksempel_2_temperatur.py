"""
Database eksempel i eksempel 2:

Denne koden laster ned temperaturdata, behandler dataen og plotter den
"""

#imporer nødvendige moduler:
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from libraries.database_library import get_messages_sync

from pandas.plotting import register_matplotlib_converters
register_matplotlib_converters()

#last ned data fra databasen:
messages = get_messages_sync(query={'address': 'temp'}, max_length=100000)

#sett opp pandas dataframe:
df = pd.DataFrame(list(messages))
print(f'Mottok {len(df)} meldinger fra database')
df.rename(columns={'message':'temperatur'}, inplace=True)

#slett unødvendige kolonner:
df.drop(['author', 'address', '_id'], axis=1, inplace=True)

#konverter temperaturmålinger til float:
df['temperatur'] = pd.to_numeric(df['temperatur'], downcast='float', errors='coerce')

#slett ikke-gyldige verdier:
df.dropna(inplace=True)

#sett tid som index:
df.set_index('time', inplace=True)

#hent verdier fra dag:
dato = '12-2020'
dag = df[dato]
snitt = dag.resample('5T').mean()   #snitt hvert femte minutt
snitt_max= dag.resample('H').max()  #maxverdi for hver time
snitt_min = dag.resample('H').min() #minverdi for hver time


print(f'Det ble hentet {len(df)} meldinger fra databasen.')
print(f'Det er {len(dag)} målinger for {dato}.')

#lag et plot for å vise at dataene er der:

fig, ax = plt.subplots(1)
fig.autofmt_xdate()
ax.plot(snitt.index, snitt.temperatur)
ax.fill_between(snitt_max.index, snitt_max.temperatur, snitt_min.temperatur, alpha=0.2)

xfmt = mdates.DateFormatter('%d kl%H:%M')
ax.xaxis.set_major_formatter(xfmt)
plt.legend(('Snitt hvert femte minutt', 'Maximum og minimumsverdi hver time'))
plt.xlabel("Dag og klokkeslett for måling")
plt.ylabel("Temperatur (i Celsius)")
plt.title('Temperaturmålinger eksempel 2')
plt.show()

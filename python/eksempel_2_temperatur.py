"""
Database eksempel i eksempel 2:

Denne koden laster ned temperaturdata, behandler dataen og plotter den
"""

#imporer nødvendige moduler:
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from libraries.database_library import get_messages_sync

#last ned data fra databasen:
messages =  get_messages_sync(query={'address': 'Sindre'}, max_length=10000)

#sett opp pandas dataframe:
df = pd.DataFrame(list(messages))
df.rename(columns={'message':'temperatur'}, inplace=True)

#slett unødvendige kolonner:
df.drop(['author', 'address', '_id'], axis=1, inplace=True)

#konverter temperaturmålinger til float:
df['temperatur'] = pd.to_numeric(df['temperatur'], downcast='float', errors='coerce')

#slett ikke-gyldige verdier:
df.dropna(inplace=True)

#sett tid som index:
df.set_index('time', inplace=True)

#lag et plot for å vise at dataene er der:
df.plot()
plt.title('Temperaturmålinger eksempel 1')
plt.show()

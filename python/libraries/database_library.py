"""
kontroll av database i prosjekt i IELET2001.
scriptet inneholder funksjoner for å kontrollere en mongodb database, hostet
av mongodb
"""

from datetime import datetime
import motor.motor_asyncio
from pymongo import MongoClient

#global uri for database:
PASSWORD = 'blonk-admin'
DBNAME = 'blonkdb'
URI = f'mongodb+srv://blonkserver:{PASSWORD}@cluster0.etj0n.mongodb.net/{DBNAME}?retryWrites=true&w=majority'

class Database:
    """
    database klasse
    """
    def __init__(self):
        """
        lagre klient og db objekt
        """
        self.client = motor.motor_asyncio.AsyncIOMotorClient(URI)
        self.db = self.client.socketPong

    async def save_alias(self, alias, author):
        """
        lagre et navn for en adresse i alias databasen
        """
        document = {'name': alias, 'author': author, 'time': datetime.utcnow()}
        result = await self.db.alias.insert_one(document)
        return result

    async def save_message(self, address, message, author):
        """
        lagre meldinger sendt til serveren på eget format
        """
        document = {'address': address, 'message': message,
                    'author': author, 'time': datetime.utcnow()}
        result = await self.db.messages.insert_one(document)
        return result

    async def get_last_message(self, address):
        """
        hent siste meldingen til en spesifik addresse.
        henter siste melding basert på tidspunkt meldingen ble lagret
        """
        cursor = self.db.messages.find({'address': address})
        cursor.sort('time', -1).limit(1)
        document = await cursor.to_list(length=1)
        last_message = document[0]['message']
        return last_message

    async def get_messages(self, query={}, max_length=100):
        """
        henter så mange meldinger du vil :)
        """
        cursor = self.db.messages.find(query).sort('time', -1).limit(max_length)
        for document in await cursor.to_list(length=max_length):
            yield document

    async def get_aliases(self, query={}, max_length=100, time_sort=-1):
        """
        henter dokumenter i alias database
        """
        cursor = self.db.alias.find(query).sort('time', time_sort).limit(max_length)
        for document in await cursor.to_list(length=max_length):
            yield document

    async def get_messages_by_alias(self, alias, max_length=100):
        """
        hent alle meldinger sendt fra et spesifikt alias
        """
        query = {'name': alias}
        alias_docs = [doc async for doc in self.get_aliases(query=query)]

        authors = []
        times = []
        for doc in alias_docs:
            new_author = doc['author']
            if len(authors) == 0:
                authors.append(new_author)
                times.append(doc['time'])

            elif authors[-1] != new_author:
                authors.append(new_author)
                times.append(doc['time'])

        for i, author in enumerate(authors):
            start_time = times[i]
            query = {'author': author, 'time': {'$gt': start_time}}
            next_aliases = [doc async for doc in self.get_aliases(query=query, max_length=1, time_sort=1)]
            if len(next_aliases) > 0:
                stop_time = next_aliases[0]['time']
            else:
                if i > 0:
                    stop_time = times[i-1]

                else:
                    stop_time = datetime.utcnow()

            query = {'author': author,
                     'time': {'$gte': start_time}, 'time': {'$lt': stop_time}}
            async for document in self.get_messages(query=query):
                yield document

def get_messages_sync(query={}, max_length=100):
    #connect to database:
    client = MongoClient(URI)
    db = client.test
    collection = db.messages
    cursor = collection.find(query).sort('time', -1).limit(max_length)
    for document in cursor:
        yield document

if __name__ == '__main__':
    import asyncio
    from pprint import pprint
    async def testing():
        database = Database()
        async for document in database.get_messages():
            pprint(document)

    LOOP = asyncio.get_event_loop()
    LOOP.run_until_complete(testing())

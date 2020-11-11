"""
kontroll av database i prosjekt i IELET2001.
scriptet inneholder funksjoner for å kontrollere en mongodb database, hostet
av mongodb
"""

from datetime import datetime
import motor.motor_asyncio

class Database:
    """
    database klasse
    """
    password = 'blonk-admin'
    dbname = 'blonkdb'
    uri = f'mongodb+srv://blonkserver:{password}@cluster0.etj0n.mongodb.net/{dbname}?retryWrites=true&w=majority'

    def __init__(self):
        """
        lagre klient og db objekt
        """
        self.client = motor.motor_asyncio.AsyncIOMotorClient(self.uri)
        self.db = self.client.test

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

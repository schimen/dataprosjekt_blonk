# idIOT

Dette er github repository til idIOT, gruppe 23 i dataprosjekt IELT2001 høst 2020.
Vi har laget en IoT platform basert på en raspberry pi server, esp32 klienter og kommunikasjon gjennom websockets.

Se readme i de python og arduino mappe for installasjons-veiledning. Her er oversikt over mappene:

arduino\
|--Bibliotek\
|  |--Arduino-idIOT\
|  |  |--Arduino-idIOT.h
|  |  |--Arduino-idIOT.cpp
|  |--ArduinoWebsockets (eksternt bibliotek)
|--Eksempler/
|  |--LED/
|  |  |--LED.ino
|  |--Pong/
|  |  |--Pong.ino
|  |  |--TFT-eSPI (eksternt bibliotek)
|  |--Termostat/
|  |  |--Termostat.ino
|  |--live_plot_test/
|     |--live_plot_test.ino
|--README.md

python/
|--libraries/
|  |--client_library.py
|  |--database_library.py
|  |--gui_library.py
|--websocket_server.py
|--eksempel_LED.py
|--eksempel_termostat.py
|--eksempel_live_plot.py
|--eksempel_enkel_kontrollpanel.py
|--klient_kontrollpanel.py
|--wireshark_capture_script.py
|--README.md

Nødvendige biblioteker er:
* python:
	* [websockets](https://pypi.org/project/websockets/)
	* [pymongo](https://pypi.org/project/pymongo/)
	* [motor](https://pypi.org/project/motor/)
	* [matplotlib](https://pypi.org/project/matplotlib/)
* arduino:
	* [ArduinoWebsockets](https://github.com/gilmaimon/ArduinoWebsockets)
	* [TFT_eSPI](https://www.arduino.cc/reference/en/libraries/tft_espi/)

## Gruppe 23 dataprosjekt IELET2001 høst 2020

IOT is the F U T U R E
![](Dokumentasjon/Bilder/idIOT-logo-transparent.png)



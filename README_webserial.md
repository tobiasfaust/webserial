# Webserial Library für ESP-Projekte

## Übersicht

Die Webserial-Library ermöglicht die komfortable Anzeige von Log-Ausgaben eines ESP32/ESP8266 im Browser über eine WebSocket-Verbindung. Die Logdaten werden per WebSocket an eine HTML/JS-Oberfläche übertragen und dort mit Zeilennummern, Download- und Clear-Funktion dargestellt.

**Voraussetzung:**  
Das Zielprojekt muss eine WebSocket-Verbindung bereitstellen, die auf das `subscribe`-Kommando reagiert und Logdaten sendet.

## Integration in ein ESP-Projekt

### 1. Einbindung über [`platformio.ini`](platformio.ini)

Füge das Webserial-Repository als Abhängigkeit in deiner [`platformio.ini`](platformio.ini) hinzu:

```ini
lib_deps =
    https://github.com/tobiasfaust/webserial.git
```

### 2. Automatisches Deployment der Webserial-Dateien

Das Skript [`scripts/copy_data_files.py`](scripts/copy_data_files.py) sorgt dafür, dass die Webserial-HTML/JS-Dateien automatisch in den [`data`](data)-Ordner deines Zielprojekts kopiert werden.  
Du musst nichts weiter tun – beim Build werden die Dateien übernommen.

### 3. Webserver und WebSocket einrichten

Nutze z.B. die [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) Library, um einen Webserver und WebSocket bereitzustellen.

### 4. WebSocket-Handler für `subscribe`-Kommando

Der WebSocket muss auf folgende eingehende Nachrichte reagieren und bei `subscribe` die Logausgaben regelmäßig an den Client senden.

**Achtung:**
Die Subscribe-Nachricht wird als JSON gesendet:

```json
{
    "cmd": {
        "action": "subscribe",
        "subaction": "log_data",
        "callbackFn": "MyCallback"
    }
}
```


Die Antwort des ESP muss immer ein JSON sein, das folgende Attribute enthält:
- `logline`: Die Logzeile als String
- `callbackFn`: Der Wert aus `"cmd" : "callbackFn"` der Anfrage (1:1 zurückgeben)

**Beispiel für die Antwort:**

```json
{
  "logline": "Logzeile 1",
  "callbackFn": "MyCallback"
}
```

## 5. webserial html einbinden
Wenn die Websocketadresse nicht `/ws` ist, muss webserial über einen Wrapper die korrekte Adresse mitgeteilt werden. Dazu muss der Parameter `ws_url` gesetzt werden. Ein Beispiel dazu findest du im `examples/webserial_frame.html`.  

## Beispiel: 
siehe `examples/demo.ino`

## Webserial im Browser nutzen

1. Flashe das Projekt auf dein ESP-Gerät.
2. Öffne die IP-Adresse des ESP im Browser (z.B. `http://192.168.x.x/`).
3. Die Webserial-Oberfläche erscheint und zeigt die Logausgaben live an.

## Erweiterung: Eigene Logausgaben senden

Du kannst überall im Code `sendLogLine("Deine Lognachricht")` aufrufen, um Nachrichten an die Webserial-Oberfläche zu schicken.

## Referenz: ESP32_PumpControl

Die vollständige Implementierung findest du im Projekt [ESP32_PumpControl](https://github.com/tobiasfaust/ESP32_PumpControl), insbesondere im [src/main.cpp](https://github.com/tobiasfaust/ESP32_PumpControl/blob/main/src/main.cpp) und im Webserver-Setup.

---

**Weitere Informationen:**
- [ESPAsyncWebServer Dokumentation](https://github.com/me-no-dev/ESPAsyncWebServer)
- [PlatformIO Dokumentation](https://docs.platformio.org/)
- [Webserial Repository](https://github.com/tobiasfaust/webserial)

---

**Dateien im Webserial-Projekt:**
- [`data/webserial.html`](data/webserial.html)
- [`data/webserial.js`](data/webserial.js)
- [`scripts/copy_data_files.py`](scripts/copy_data_files.py)

---

**Beispielprojekt:**  
Das Beispiel kann als `src/example.ino` in deinem Zielprojekt verwendet werden.  
Vergiss nicht, SPIFFS zu initialisieren, falls du die Dateien aus dem [`data`](data)-Ordner bereitstellst.

---

**Viel Erfolg beim Einbinden und Nutzen von Webserial!**

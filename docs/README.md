# 🧠 Sensor Fusion Headtracking

Ein exploratives AR/XR-Projekt zur Demonstration von Head-Tracking durch **Sensorfusion** – umgesetzt mit C++ für die Sensordatenerfassung und **Unity** zur 3D-Visualisierung der Kopfbewegungen. Ziel ist es, durch ein öffentliches Tech-Demo-Projekt praktische Erfahrung mit Sensorfusion und Echtzeit-Datenvisualisierung aufzubauen.

## 🎯 Ziel des Projekts

Dieses Projekt zeigt, wie IMU-Daten (Gyro, Accelerometer, ggf. Magnetometer) mithilfe von Sensorfusion (Kalman Filter) zu einer stabilen 3D-Orientierung verrechnet werden. Diese Ausrichtung wird über UDP an eine Unity-Anwendung gesendet und dort in Echtzeit visualisiert – z. B. zur Darstellung eines Kopfmodells.

---

## 🧱 Projektstruktur

```bash
sensor-fusion-headtracking/
├── cpp_firmware/              # C++-Code für Sensorfusion und Datenübertragung
│   ├── i2c_mpu9250.cpp        # I2C-Kommunikation mit dem Sensor (MPU9250)
│   ├── sensor_fusion.cpp      # Platzhalter für Fusionsalgorithmus
│   ├── orientation.cpp        # Platzhalter für Euler-/Quaternion-Konvertierung
│   └── udp_sender.cpp         # Senden der Sensordaten über UDP
│
├── unity_visualization/       # Unity-Projekt für die 3D-Visualisierung
│   └── ...                    # Kamera, 3D-Modell, UDP-Receiver etc.
│
├── docs/                      # Dokumentation, ggf. Diagramme
│   └── README.md              # Dieses Dokument
│
└── .gitignore                 # Ignorierte Build-/Systemdateien
```

---

## ⚙️ Verwendete Technologien

### C++ Firmware
- Zugriff auf MPU9250 über I2C
- Eigenständige Sensordatenerfassung und -verarbeitung
- UDP-Socket für Datenübertragung

### Unity Visualisierung
- Empfang und Interpretation der Sensordaten (z. B. Quaternionen)
- Darstellung eines Kopfmodells in Echtzeit
- Grundlage für AR/XR-Experimente

---

## 🚀 Nächste Schritte / Roadmap

- [ ] Sensorfusion implementieren (z. B. Complementary Filter)
- [ ] UDP-Datenpaketstruktur finalisieren
- [ ] Unity-UDP-Receiver mit Live-3D-Update implementieren
- [ ] Erste Tests mit realer Hardware
- [ ] Optional: Erweiterung um BLE/WiFi-Streaming

---

## 👨‍💻 Autor

**Patrick Eichler**  
Embedded & Software Engineer mit Fokus auf Reglerentwicklung, Sensorik und technischer Visualisierung. Dieses Projekt entstand aus dem Interesse an Echtzeit-Orientierungstracking und dient als technisches Experiment im Bereich **AR/XR** und **Sensorfusion**.


---

## 📄 Lizenz

MIT License – frei zur Nutzung, Weiterentwicklung und Einbindung. Gerne mit Link auf das Originalprojekt.

# Physiological Stress-Response Monitor

Real-time heart rate and peripheral temperature monitoring during standardized cognitive stress tasks. Built with an **ESP32**, a **MAX30102** optical sensor and a **DS18B20** temperature sensor, with a browser-based app that guides each session and exports labeled data for analysis.

> Educational research prototype. Not a medical device.

## What it does

The system records heart rate (BPM) and peripheral temperature at the same time while a person does a cognitive task. The hypothesis: under stress, heart rate rises and peripheral temperature falls (sympathetic vasoconstriction), so the two should be negatively correlated.

```
MAX30102 (I2C) ─┐
                ├─> ESP32 ──WiFi / WebSocket (port 81, JSON every 1.5 s)──> Web app (browser)
DS18B20 (1-Wire)┘                                                          │
                                                                           └─> CSV export
```

## Hardware

| Sensor pin | ESP32 pin |
| --- | --- |
| MAX30102 VIN / GND | 3.3V / GND |
| MAX30102 SDA / SCL | GPIO 21 / GPIO 22 |
| DS18B20 VCC / GND | 3.3V / GND |
| DS18B20 DATA | GPIO 4 (plus a 4.7 kΩ pull-up between VCC and DATA) |

Both sensors run at 3.3 V. Total hardware cost is under 300 MXN.

## Firmware (`firmware/stress_monitor`)

C++ on the Arduino IDE. Libraries: WebSocketsServer (Markus Sattler), ArduinoJson, SparkFun MAX3010x, OneWire, DallasTemperature.

- Beat detection with SparkFun's `checkForBeat()`; BPM computed per beat and filtered to 40–200.
- 20-beat circular average plus exponential smoothing for the reported value.
- Finger detection from the raw IR signal (threshold 50,000).
- JSON message every 1.5 s: `{"bpm": 78, "temp": 34.6, "finger": true, "raw_ir": 102450}`

## Web app (`webapp/`)

Single-page app in plain HTML, CSS and JavaScript (Chart.js for live charts), mobile-first. Chart.js is loaded from a CDN, so the device needs internet access the first time (a local copy of Chart.js would make it fully offline). Four screens:

1. **Setup:** anonymous subject ID, ESP32 IP, task selection, phase durations.
2. **Sensor stabilization:** session unlocks when HR varies ≤ 5 BPM across 8 samples and temperature ≤ 0.3 °C.
3. **Live session:** rest, stress, recovery phases with charts and a timer.
4. **Results:** per-phase means, Pearson correlation, task accuracy, CSV export.

Cognitive tasks: **Stroop** (1.8 s per item), **1-Back** (2 s per item), **PASAT-style arithmetic** (3.5 s per item).

Stress is induced by more than the task itself: strict per-item time limits, a visible accuracy target (70%), a countdown timer, streak counter, sounds, screen flashes and occasional pressure messages. Keep this in mind when interpreting the physiological response.

## Experimental protocol

| Phase | Default duration | Purpose |
| --- | --- | --- |
| Rest | 120 s | Baseline |
| Cognitive stress | 180 s | Induce sympathetic activation |
| Recovery | 120 s | Observe return to baseline |

CSV columns: `timestamp`, `elapsed_s`, `fase`, `bpm`, `temperatura`, `test_correcto`.

Notes on the data: the first lines of each file start with `#` (subject ID, task, date), so read them with `comment='#'` in pandas. Samples are logged every time the ESP32 sends a message (about every 1.5 s) while a finger is detected, and each task answer adds an extra row with `test_correcto` set to 0 or 1. Analyses should treat these two kinds of rows separately.

## Findings so far

Exploratory analysis of 6 sessions (one participant each; five Stroop, one PASAT) in [`analysis/stress_analysis.ipynb`](analysis/stress_analysis.ipynb):

- **Heart rate responded to the task in most sessions:** a peak 10-s rise of +10 to +30 BPM at stress onset in 5 of 6 sessions (the PASAT session was the exception). Phase means understate this because heart rate often peaks early and then falls back while the task continues.
- **Recovery was inconsistent:** heart rate returned to rest level or below in 4 of 6 sessions and stayed above it in the other 2.
- **Temperature did not follow the phases:** in 5 of 6 sessions its trend kept the same sign through rest, stress and recovery, which looks like sensor/room drift. It cannot be interpreted without a no-task control session.
- **The whole-session Pearson coefficient is not a reliable summary** (negative in only 3 of 6 sessions). A lagged analysis after detrending is suggestive (heart rate tended to be followed by lower temperature, with highly variable lags) but weak.

With n = 6, mixed tasks and no control session, these results are exploratory and not evidence about stress physiology in general.

## Known limitations and next steps

- `requestTemperatures()` blocks for about 750 ms at 12-bit resolution; switching to a non-blocking read would avoid missing beats.
- PPG is sensitive to motion, finger pressure and ambient light; peripheral temperature needs minutes to equilibrate.
- Small, homogeneous sample; results are exploratory.
- The original technical documentation says the app needs no internet; in practice Chart.js is fetched from a CDN.
- Planned: more sessions with default durations plus no-task control sessions, insulated temperature sensor, SQLite storage of sessions, HRV metrics (RMSSD, pNN50), SpO₂.

## Run it

1. Install the libraries above in the Arduino IDE.
2. In `firmware/stress_monitor/stress_monitor.ino`, set your own WiFi name and password. Do not commit them.
3. Flash the ESP32 and read its IP from the Serial Monitor (115200 baud).
4. Open `webapp/index.html` directly from your computer or phone (not from an HTTPS page; browsers block insecure `ws://` connections from HTTPS sites, so GitHub Pages will not work) and enter the ESP32 IP.

## Data and analysis

`data/` holds anonymized session CSVs (S01–S06; see `data/README.md` for details). `analysis/stress_analysis.ipynb` loads every CSV in `data/`, harmonizes the app versions and produces the figures in `analysis/figures/` and `analysis/session_summary.csv`. To run it: `pip install -r analysis/requirements.txt`, then open the notebook with Jupyter.

## Ethics and privacy

Subjects are identified only by anonymous IDs (S01, S02…). Share data only with participants' informed consent.

## Documentation

Full technical documentation (Spanish): `docs/documentacion_tecnica_es.md`.

## Author

Alfonso Kiyoshi López Vieyra · Biomedical Engineering, Instituto Tecnológico de Morelia

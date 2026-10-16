# Data

Anonymized session recordings, one participant per file (S01–S06). Participants are identified only by these codes. Raw files are left as exported by the app.

| ID | Task | Date | Sampling | Rest / stress / recovery (s) |
| --- | --- | --- | --- | --- |
| S01 | Stroop | 2026-04-20 | 0.5 s | 59 / 59 / 59 |
| S02 | Stroop | 2026-04-24 | 0.5 s | 60 / 90 / 60 |
| S03 | Stroop | 2026-05-21 | 0.5 s | 60 / 60 / 60 |
| S04 | Stroop | 2026-05-21 | 0.5 s | 119 / 60 / 120 |
| S05 | PASAT | 2026-05-25 | 1.5 s | 119 / 178 / 118 |
| S06 | Stroop | 2026-05-27 | 1.5 s | 58 / 119 / 58 |

## Format notes

- Columns: `timestamp`, `elapsed_s`, `fase`, `bpm`, `temperatura`, and a task-answer column (`stroop_correcto` in older app versions, `test_correcto` in newer ones).
- Newer files start with `#` metadata lines; older files do not. Read with `pandas.read_csv(path, comment="#")`.
- Sensor samples and task answers are different row types: answer rows have a value in the answer column (0 or 1), sensor rows leave it empty.
- Older app versions kept recording after recovery ended, labeled `fin`; the analysis drops those rows.
- Heart rate sometimes stays at one exact value for 20 s or more (for example S03 near the end), which suggests a held value rather than live beats.
- S01 recorded only 5 task answers, so its accuracy is not reliable.

## Consent

Data are shared only with the participants' informed consent.

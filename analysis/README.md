# Analysis

`stress_analysis.ipynb` runs the full exploratory analysis on every `S*.csv` file in `../data/`:

- harmonizes the CSV formats produced by different app versions,
- computes per-session heart-rate and temperature metrics by phase,
- compares temperature trends across phases,
- runs a detrended lagged cross-correlation between heart rate and temperature.

Outputs: figures in `figures/` and `session_summary.csv`.

Run: `pip install -r requirements.txt`, then `jupyter notebook stress_analysis.ipynb` (run it from this folder so the relative paths work).

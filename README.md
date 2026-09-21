# ACTAF data analysis – GSI/FAIR Summer Student Program 2026

ROOT/C++ macros and figures from my work on the **ACTAF** (ACtive TArget for FAIR) detector during the GSI/FAIR Summer Student Program 2026. ACTAF is a gas-filled active-target TPC (2 anodes × 64 pads = 128 pads) read out within the R3BRoot framework. Five ²⁴¹Am alpha sources inside the drift volume are used for monitoring and calibration.

The work covers three topics:

1. **Charge collection vs. high voltage** – alpha peak position (Mean) as a function of HV (runs 4122–4131, 90 % → 5 % of nominal, 4 bar; run 4035 = 100 % nominal)
2. **Noise analysis using FFT** – averaged power spectra of raw waveforms (main components at ~206, 280 and 610 kHz)
3. **Summary table for gas quality monitoring** – alpha peak parameters per pad and per run, linked to the experimental logbook

The full description of the methods and results is in the extended technical report.

## Repository contents

| File / folder | Description |
|---|---|
| `alphaPeakAnalysis.cpp` | Builds per-pad alpha integral histograms (1D spectrum and integral vs. event number) from a run |
| `openAlphaHistograms.cpp` | Opens the histograms from `alphaPeakAnalysis.cpp` and saves them as PNG images |
| `plot_pad.cpp` | Alpha peak fits for one pad across several runs; Mean vs. HV and Integral vs. HV |
| `fitAllPadsAlpha.cpp` | Crystal Ball fit of the alpha peak in all pads of one run; writes a text file with results |
| `makeTable.C` | Connects fit results with `runlog.txt` and creates the summary table |
| `waveform_Viewer.cpp` | Interactive viewer of raw waveforms with the integration window |
| `average_FFT.cpp` | Event-averaged FFT power spectrum for a selected pad |
| `fitFFT.cpp` | Automatic multi-peak search and fit of the averaged FFT spectrum |
| `runlog.txt` | Logbook: pressure, HV, beam status and comment, one line per run |
| `all_runs_table.txt` | Summary table of alpha peak parameters for all processed runs |
| `alpha_histograms_fit_result/` | Output of the alpha peak fits |
| `pictures/`, `pictures_2/` | Figures |

## Requirements

- [ROOT](https://root.cern) 6.x
- Input data are ROOT files produced by R3BRoot (`online_data_<run>_*.root`, `nearline_data_*.root`). **The data are not part of this repository** – they are stored on the GSI cluster (Lustre).

## Usage

All macros are run from the command line with ROOT:

```bash
# Alpha histograms of one run (optionally limit the number of events)
root -l 'alphaPeakAnalysis.cpp("/path/to/data.root", 100000)'

# Draw the histograms and save them as PNG
root -l 'openAlphaHistograms.cpp("alpha_histograms_<run>.root")'

# Mean vs. HV for one pad (default pad 115); edit the run list at the top of the file
root -l -q 'plot_pad.cpp(122)'

# Waveform viewer: pad, integral threshold, amplitude threshold, file
root -l 'waveform_Viewer.cpp(122, 100.0, 60.0, "/path/to/data.root")'

# Averaged FFT: pad, sampling rate [Hz], file
root -l 'average_FFT.cpp(55, 19.44e6, "/path/to/data.root")'

# Fit of the averaged FFT spectrum (threshold and sigma need to be adjusted per pad)
root -l 'fitFFT.cpp'
```

Waveform viewer controls: `n` next event, `p` previous, `j` jump to event, `s` save PNG, `q` quit.

## Gas quality table – workflow

1. `alphaPeakAnalysis.cpp` – histograms of alpha integrals for the selected pads
2. `fitAllPadsAlpha.cpp` – fit of the alpha peak in each pad, one text file per run
3. `makeTable.C` – adds the run conditions from `runlog.txt` and writes the table (single file or whole folder → `all_runs_table.txt`)

When a new measurement is taken, add one line to `runlog.txt` and repeat steps 2–3.

## Author

Patrik Doležal

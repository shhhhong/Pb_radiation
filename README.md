## MyExampleB1-Extended: Pb Radiation Study with Geant4

This project extends the official Geant4 B1 example for radiation transport studies in Pb material.
It introduces additional scoring and trajectory analysis capabilities with ROOT output.

⸻

## ✨ Features

### Geometry
- **World volume**: air  
- **Envelope volume**: lead (**Pb**) target  

### Physics
- Based on **FTFP_BERT** physics list  
- High-precision EM physics: **G4EmStandardPhysics_option4**  
- **G4StepLimiterPhysics** to control charged-particle step size  

### Primary Generator
- Default: **662 keV γ-rays** from *z = -5 cm*, collimated along **+Z**  
- Configurable with **GPS UI commands** (`/gps/particle`, `/gps/ene/*`, `/gps/pos/*`, `/gps/ang/*`)  

### Scoring
- **Energy deposition** → handled in `MyRunAction.cc`  
- **Particle trajectory** (per-step record) → handled in `MySteppingAction.cc`  
- Output stored in **ROOT ntuple** (`steps.root`)  

### Analysis
- Use **ROOT macros** to:  
  - [`EdepSpectrum.C`] → plot **energy deposition spectra**  
  - [`draw.C`] → visualize **particle trajectories** in 2D (e.g. *xz plane*)
 
---

## Installation


### To compile

cd build
cmake ..
make -j (number of threads of your PC)

### To execute without bash mode

./MyExampleB1  

### To execute in bash mode

./MyExampleB1 run1.mac


# Dynamic Delta Hedging — C++17 (g++-15)

Implements the ISyE 6767 interim project: **dynamic delta hedging** for a European call option using the **Black–Scholes (B–S)** model.

- **Task 1:** Simulate GBM stock paths, price with B–S, construct daily delta hedges, and export outputs for plotting (100 displayed paths + terminal hedging error distribution).
- **Task 2:** Use **real GOOG** data (stock, option quotes, daily risk-free rates) to compute **implied volatility (per‑day)**, build a daily delta hedge, and write a strict `result.csv`.

---

## 1) Prerequisites

- macOS with Homebrew GCC 15.x
  ```bash
  g++-15 --version
  # g++-15 (Homebrew GCC 15.1.0) 15.1.0
  ```
- Make

---

## 2) Build

```bash
make build
```
This compiles `src/main.cpp`

Binary: `bin/run`

---

## 3) Layout

```
.
├── Makefile
├── README.md
├── data/
│   ├── interest.csv
│   ├── sec_GOOG.csv
│   └── op_GOOG.csv
├── out/
│   ├── sim_paths.csv
│   ├── sim_terminal_HE.csv
|   └── result.csv
├── reports/
│   └── report_template.md
└── src/
    └── main.cpp
```

---

## 4) Task 1 — Simulation

Run:
```bash
make run-task1
```
Parameters (assignment defaults):
- `S0=100, T=0.4, μ=0.05, σ=0.24, r=0.025, N=100, paths=1000`

Outputs:
- `out/sim_paths.csv` — 101 rows (t=0..100), up to 100 columns of simulated paths (+ `t_index`)
- `out/sim_terminal_HE.csv` — 1000 terminal cumulative hedging errors (`HE_T`)

### Plotting (Python)
From project root:
```bash
python plot_task1.py
```
Creates (under `out/plots/`):
- `plots_task1_paths.png` — **100** simulated paths (or fewer if fewer columns)
- `plots_task1_he_hist.png` — histogram of terminal hedging errors
- `plots_task1_he_stats.txt` — summary stats (count/mean/std/min/max/p5/p50/p95)

You can override paths if needed:
```bash
python plot_task1.py   --paths out/sim_paths.csv   --he out/sim_terminal_HE.csv   --outdir out/plots   --max_paths 100
```

---

## 5) Task 2 — Real Data (Strict `result.csv`)

### Data
Place the provided CSVs into `data/`:
- **interest.csv** — daily risk‑free rate, annualized percent (`0.29` → 0.29% → 0.0029 decimal)
- **sec_GOOG.csv** — GOOG adjusted close prices; columns include `date`, and `close adj` (or similar)
- **op_GOOG.csv** — option quotes with at least: `date`, `exdate`, `cp_flag` (C/P), `strike/strk`, `best_bid/bid`, `best_offer/ask` (or `mid`)

Column names are matched case‑insensitively and tolerantly.

### Run
```bash
make run-task2
```
Contract (per assignment example):
- Window: `t0 = 2011-07-05` to `tN = 2011-07-29` (weekdays intersected across all datasets)
- Option: Call with `K = 500`, `exdate = 2011-09-17`

**Output:** `out/result.csv` **with exactly**:
```
date,S,V,iv,delta,PNL,PNL_with_hedge
```
Definitions:
- `S` — GOOG adjusted close
- `V` — option mid price
- `iv` — implied volatility from B–S bisection
- `delta` — `N(d1)` using that `iv`
- `PNL` — `V0 - V(date)`
- `PNL_with_hedge` — cumulative hedging error up to `date`

First row uses `PNL=0` and `PNL_with_hedge=0`.

---

## 6) Math & Method

- **B–S (call):**
  - `d1 = [ln(S/K) + (r + 0.5σ²)τ] / (σ√τ)`, `d2 = d1 − σ√τ`
  - `C = S·N(d1) − K·e^(−rτ)·N(d2)`, `delta = N(d1)`
- **Implied vol:** solve `C_BS − V_mkt = 0` by **bisection** (`σ ∈ [1e−6, 5.0]`, tol `1e−7`).
- **Hedging recursion:**
  - `B0 = V0 − δ0 S0`
  - `Bi = δ_{i−1} S_i + B_{i−1} e^{r_{i−1} Δt} − δ_i S_i`, with `Δt = 1/252`
  - `HE_i = δ_{i−1} S_i + B_{i−1} e^{r_{i−1} Δt} − V_i`
  - `PNL_i = V0 − V_i`, `PNL_with_hedge_i = HE_i`

---

## 7) Self‑Tests

```bash
make test
```
Smoke tests validate:
- B–S price vs a known reference
- IV round‑trip (price → IV → price)
- Delta monotonicity in `S`

---

## 8) Troubleshooting

- **Empty `result.csv`:** Check that each date between `t0`..`tN` exists in **all** three datasets and that the specified `(K, exdate, cp=C)` option is quoted each day.
- **NaN `iv`:** Market price may be outside B–S bounds; ensure `mid` is valid for that date.
- **Plots missing:** Only CSVs are produced; generate plots externally for the report.

---

## 9) Rubric Alignment

- Task 1 outputs for plotting, terminal hedging error distribution
- Task 2 strict `result.csv` and daily IV/Δ
- Report template provided in `reports/` (fill after running code with plots and test summaries)
# Dynamic-Delta-Hedging
# Dynamic-Delta-Hedging

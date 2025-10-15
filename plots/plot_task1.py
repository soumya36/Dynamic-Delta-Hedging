#!/usr/bin/env python3
import argparse
import os
import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def err(msg):
    print(f"[ERROR] {msg}", file=sys.stderr)
    sys.exit(1)

def main():
    parser = argparse.ArgumentParser(
        description="Task 1 plots: GBM paths and terminal hedging error histogram."
    )
    # Defaults aligned with your repo layout in the screenshot:
    parser.add_argument("--paths", type=str, default="out/sim_paths.csv",
                        help="CSV with simulated paths (default: out/sim_paths.csv)")
    parser.add_argument("--he", type=str, default="out/sim_terminal_HE.csv",
                        help="CSV with terminal hedging errors (default: out/sim_terminal_HE.csv)")
    parser.add_argument("--outdir", type=str, default="out/plots",
                        help="Output directory for plots and stats (default: out/plots)")
    parser.add_argument("--max_paths", type=int, default=100,
                        help="Max number of paths to plot (default: 100)")
    args = parser.parse_args()

    os.makedirs(args.outdir, exist_ok=True)

    # 1) Plot up to 100 simulated paths
    if not os.path.exists(args.paths):
        err(f"Paths CSV not found at: {args.paths}")
    paths_df = pd.read_csv(args.paths)

    if "t_index" not in paths_df.columns:
        err("'t_index' column not found in paths CSV.")

    path_cols = [c for c in paths_df.columns if c.startswith("path_")]
    if not path_cols:
        err("No columns starting with 'path_' found in paths CSV.")
    path_cols = path_cols[: args.max_paths]

    plt.figure()
    x = paths_df["t_index"].values
    for c in path_cols:
        y = paths_df[c].values
        plt.plot(x, y, linewidth=0.8)
    plt.xlabel("Time step (t index)")
    plt.ylabel("Simulated Price S_t")
    plt.title(f"Simulated GBM Stock Price Paths ({len(path_cols)} shown)")
    out_paths_png = os.path.join(args.outdir, "plots_task1_paths.png")
    plt.tight_layout()
    plt.savefig(out_paths_png, dpi=150)
    plt.close()

    # 2) Histogram of terminal hedging errors
    if not os.path.exists(args.he):
        err(f"Hedging error CSV not found at: {args.he}")
    he_df = pd.read_csv(args.he)

    if "HE_T" not in he_df.columns:
        err("'HE_T' column not found in hedging error CSV.")

    he_vals = he_df["HE_T"].values
    plt.figure()
    plt.hist(he_vals, bins=40)
    plt.xlabel("Terminal Hedging Error (HE_T)")
    plt.ylabel("Frequency")
    plt.title(f"Distribution of Terminal Hedging Errors (n={len(he_vals)})")
    out_he_png = os.path.join(args.outdir, "plots_task1_he_hist.png")
    plt.tight_layout()
    plt.savefig(out_he_png, dpi=150)
    plt.close()

    # 3) Summary stats -> text file
    summary = {
        "count": int(len(he_vals)),
        "mean": float(np.mean(he_vals)) if len(he_vals)>0 else float("nan"),
        "std": float(np.std(he_vals, ddof=1)) if len(he_vals) > 1 else float("nan"),
        "min": float(np.min(he_vals)) if len(he_vals)>0 else float("nan"),
        "max": float(np.max(he_vals)) if len(he_vals)>0 else float("nan"),
        "p5": float(np.percentile(he_vals, 5)) if len(he_vals)>0 else float("nan"),
        "p50": float(np.percentile(he_vals, 50)) if len(he_vals)>0 else float("nan"),
        "p95": float(np.percentile(he_vals, 95)) if len(he_vals)>0 else float("nan"),
    }
    out_stats = os.path.join(args.outdir, "plots_task1_he_stats.txt")
    with open(out_stats, "w") as f:
        for k, v in summary.items():
            f.write(f"{k}: {v}\n")

    print("Wrote:")
    print(" -", out_paths_png)
    print(" -", out_he_png)
    print(" -", out_stats)

if __name__ == "__main__":
    main()

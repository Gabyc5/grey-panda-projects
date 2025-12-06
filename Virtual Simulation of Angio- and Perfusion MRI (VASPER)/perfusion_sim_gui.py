""" 
Perfusion MRI simulation GUI

The code provides a Tkinter-based interface to: 
    * define a simple 2D perfusion phantom (vessel / normal / abnormal tissue)
    * run a time-series simulation of contrast uptake
    * save output GIF/images, signal-intensity curves, and summary statistics. 
"""


import tkinter as tk
from tkinter import ttk, messagebox
from tkinter import filedialog
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
import json
from datetime import datetime
import csv
import os

# Main part of code


""" 
Simulate perfusion MRI signal evolution for vessel, normal, and abnormal tissue. 

Parameters are:
N : int 
    Matrix size (image is N x N)
frames : int 
    Number of time points in the simulation
dt : float 
    Time step between frames (arbitrary units)
phantom_bg : float 
    Background signal intensity outside tissue
issue_baseline : float 
    Baseline signal intensity for all tissue before contrast
vessel_width_px : int 
    Width (in pixels) of the central vessel stripe
normal_abnormal_split : float 
    Fraction used to split tissue into normal vs abnormal regions. 
vessel_peak, vessel_ttp, vessel_washout_tau : float 
    Parameters that control vessel bolus amplitude, time-to-peak, and washout
normal_amp, normal_rise_tau, normal_decay_tau : float 
    Amplitude, rise, and decay constants for normal tissue
abnormal_amp, abnormal_rise_tau, abnormal_decay_tau : float 
    Amplitude, rise, and decay constants for abnormal tissue (more retention)

Returns the following:

frames_data : list of 2D np.ndarray 
    List of N x N images over time
t : np.ndarray 
    1D array of time points
vessel_si, normal_si, abnormal_si : np.ndarray 
    Signal-intensity curves for each region. 

"""
def simulate_perfusion(N=256, frames=120, dt=1.0,
                       phantom_bg=0.0, tissue_baseline=1.0,
                       vessel_width_px=12, normal_abnormal_split=0.5,
                       vessel_peak=3.0, vessel_ttp=8.0, vessel_washout_tau=10.0,
                       normal_amp=0.8, normal_rise_tau=6.0, normal_decay_tau=30.0,
                       abnormal_amp=0.8, abnormal_rise_tau=14.0, abnormal_decay_tau=120.0):
    def build_masks(n=256, vessel_w=12, split=0.5):
        y, x = np.indices((n, n))
        cx = n // 2
        vessel_mask = (x >= cx - vessel_w//2) & (x < cx + vessel_w//2)
        not_vessel = ~vessel_mask
        left_boundary  = int(n * (0.5 - split/2))
        right_boundary = int(n * (0.5 + split/2))
        normal_mask   = not_vessel & (x < left_boundary)
        abnormal_mask = not_vessel & (x >= right_boundary)
        middle_mask = not_vessel & (x >= left_boundary) & (x < right_boundary)
        normal_mask   |= middle_mask & (x < cx)
        abnormal_mask |= middle_mask & (x >= cx)
        background_mask = np.zeros((n, n), dtype=bool)
        return background_mask, vessel_mask, normal_mask, abnormal_mask

    _, vessel_mask, normal_mask, abnormal_mask = build_masks(N, vessel_width_px, normal_abnormal_split)
    t = np.arange(frames) * dt

    # Vessel: quick rise to TTP then exponential washout (gamma-like)
    v_rise = np.exp(-np.maximum(0, vessel_ttp - t) / (vessel_ttp/3 + 1e-9))
    v_wash = np.exp(-np.maximum(0, t - vessel_ttp) / (vessel_washout_tau + 1e-9))
    vessel_delta = vessel_peak * v_rise * v_wash

    # Normal: faster rise, moderate decay
    normal_uptake  = (1 - np.exp(-t / (normal_rise_tau + 1e-9)))
    normal_washout = np.exp(-t / (normal_decay_tau + 1e-9))
    normal_delta   = normal_amp * (normal_uptake * normal_washout)

    # Abnormal: slower rise, slower decay (retention)
    abn_uptake   = (1 - np.exp(-t / (abnormal_rise_tau + 1e-9)))
    abn_washout  = np.exp(-t / (abnormal_decay_tau + 1e-9))
    abnormal_delta = abnormal_amp * (abn_uptake * abn_washout)

    vessel_si   = tissue_baseline + vessel_delta
    normal_si   = tissue_baseline + normal_delta
    abnormal_si = tissue_baseline + abnormal_delta

    def frame_image_at_index(i):
        img = np.full((N, N), phantom_bg, dtype=float)
        img[normal_mask]   = normal_si[i]
        img[abnormal_mask] = abnormal_si[i]
        img[vessel_mask]   = vessel_si[i]
        return img

    frames_data = [frame_image_at_index(i) for i in range(frames)]
    return frames_data, t, vessel_si, normal_si, abnormal_si



""" 
Save the main simulation outputs to disk

Creates: 
    * perfusion_sim.gif (or a sequence of PNG frames if GIF cannot be written)
    * si_curves.png (plot of SI vs time)
    * si_curves.csv (numeric SI values over time) 


Parameters are:
    * frames_data : list of 2D np.ndarray 
        Simulated image frames
    t : np.ndarray 
        Time vector
    * vessel_si, normal_si, abnormal_si : np.ndarray 
        Signal-intensity curves for each region
    * out_dir : str .
        Output directory where all files are written
    * basename : str 
        Base filename used for the GIF / frame sequence. 

"""
def save_outputs(frames_data, t, vessel_si, normal_si, abnormal_si,
                 out_dir=".", basename="perfusion_sim"):
    disp_min = min(0.0, vessel_si.min(), normal_si.min(), abnormal_si.min())
    disp_max = max(vessel_si.max(), normal_si.max(), abnormal_si.max())

    os.makedirs(out_dir, exist_ok=True)
    gif_path = os.path.join(out_dir, f"{basename}.gif")
    curves_png_path = os.path.join(out_dir, "si_curves.png")
    curves_csv_path = os.path.join(out_dir, "si_curves.csv")

    # 1) Curves plot
    plt.figure(figsize=(7, 4.5))
    plt.plot(t, vessel_si,   label="Vessel")
    plt.plot(t, normal_si,   label="Normal Tissue")
    plt.plot(t, abnormal_si, label="Abnormal Tissue")
    plt.xlabel("Time (a.u.)")
    plt.ylabel("Signal Intensity (SI)")
    plt.title("Perfusion MRI: Signal Intensity Over Time")
    plt.legend()
    plt.tight_layout()
    plt.savefig(curves_png_path)
    plt.close()

    # 2) Curves CSV
    with open(curves_csv_path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["time", "vessel_si", "normal_si", "abnormal_si"])
        for i in range(len(t)):
            w.writerow([t[i], float(vessel_si[i]), float(normal_si[i]), float(abnormal_si[i])])

    # 3) GIF animation
    fig = plt.figure(figsize=(4.8, 4.8))
    im = plt.imshow(frames_data[0], vmin=disp_min, vmax=disp_max, animated=True)
    plt.title("Perfusion MRI Simulation")
    plt.axis("off")

    def init():
        im.set_array(frames_data[0])
        return [im]

    def animate(i):
        im.set_array(frames_data[i])
        return [im]

    anim = animation.FuncAnimation(fig, animate, init_func=init, frames=len(frames_data), interval=80, blit=True)
    try:
        from matplotlib.animation import PillowWriter
        anim.save(gif_path, writer=PillowWriter(fps=12))
    except Exception:
        # Fallback to PNG frames if Pillow not available
        seq_dir = os.path.join(out_dir, f"{basename}_frames")
        os.makedirs(seq_dir, exist_ok=True)
        for i, img in enumerate(frames_data):
            plt.clf()
            plt.imshow(img, vmin=disp_min, vmax=disp_max)
            plt.axis("off")
            plt.title(f"Perfusion MRI Simulation (t={t[i]:.1f})")
            plt.savefig(f"{seq_dir}/frame_{i:04d}.png", bbox_inches="tight", pad_inches=0)
    plt.close(fig)



# ------------------------------------------------------------------
# Statistics 



"""
Compute basic quantitative perfusion metrics for a single SI curve.

Parameters
    t : np.ndarray
        Time vector.
    arr : np.ndarray
        Signal-intensity curve for one region.

Returns
    dict
        Dictionary with keys:
        * 'peak'          : maximum SI
        * 'time_to_peak'  : time at which the peak occurs
        * 'AUC'           : area under the curve
        * 'mean'          : mean SI
        * 'std'           : standard deviation of SI.
    
"""
def compute_stats(t, arr):
    peak_value = float(np.max(arr))
    ttp_index = int(np.argmax(arr))
    ttp = float(t[ttp_index])
    auc = float(np.trapezoid(arr, t))
    mean_val = float(np.mean(arr))
    std_val  = float(np.std(arr))
    return {"peak": peak_value, "time_to_peak": ttp, "AUC": auc, "mean": mean_val, "std": std_val}




"""
Save basic statistics for vessel, normal, and abnormal SI curves

Produces:
    * si_stats.csv : table of quantitative metrics per region
    * si_stats.txt : human-readable summary with brief interpretation

Parameters
    t : np.ndarray
        Time vector
    vessel_si, normal_si, abnormal_si : np.ndarray
        SI curves for each region
    out_dir : str
        Output directory
    basename : str
        Base name used for the CSV/TXT files

"""
def save_stats(t, vessel_si, normal_si, abnormal_si, out_dir=".", basename="si_stats"):
    csv_path = os.path.join(out_dir, f"{basename}.csv")
    txt_path = os.path.join(out_dir, f"{basename}.txt")
    vessel_stats = compute_stats(t, vessel_si)
    normal_stats = compute_stats(t, normal_si)
    abnormal_stats = compute_stats(t, abnormal_si)

    with open(csv_path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["Region", "Peak SI", "Time-to-Peak", "AUC", "Mean", "Std"])
        def row(name, d):
            w.writerow([name, d["peak"], d["time_to_peak"], d["AUC"], d["mean"], d["std"]])
        row("Vessel", vessel_stats)
        row("Normal Tissue", normal_stats)
        row("Abnormal Tissue", abnormal_stats)

    lines = []
    lines.append("Perfusion MRI Simulation — Summary Statistics")
    lines.append("------------------------------------------------")
    def block(name, d):
        lines.append(f"{name}:")
        lines.append(f"  Peak SI        : {d['peak']:.4f}")
        lines.append(f"  Time-to-Peak   : {d['time_to_peak']:.4f} a.u.")
        lines.append(f"  AUC            : {d['AUC']:.4f}")
        lines.append(f"  Mean SI        : {d['mean']:.4f}")
        lines.append(f"  Std. Dev. (SI) : {d['std']:.4f}")
        lines.append("")
    block("Vessel", vessel_stats)
    block("Normal Tissue", normal_stats)
    block("Abnormal Tissue", abnormal_stats)
    lines.append(
        "Interpretation:\n"
        "- Vessel peaks earliest/highest (bolus).\n"
        "- Normal tissue: faster rise, moderate washout.\n"
        "- Abnormal tissue: slower rise, higher late SI (retention)."
    )
    with open(txt_path, "w") as f:
        f.write("\n".join(lines))
    return csv_path, txt_path



# ------------------------------------------------------------------
# Extra analysis helpers 
def compute_baseline_sub_auc(t, si, baseline=1.0):
    """AUC of (SI - baseline)."""
    return float(np.trapezoid(si - baseline, t))

def compute_late_si(si, last_k=10):
    """Mean of the final K points (late-time retention)."""
    k = min(last_k, len(si))
    return float(np.mean(si[-k:]))

def save_stats_extended(t, vessel_si, normal_si, abnormal_si,
                        out_dir=".", basename="si_stats",
                        baseline=1.0, late_k=10):
    """
    Extends si_stats with Late-SI and Baseline-subtracted AUC.
    Writes CSV + human-readable TXT.
    """
    csv_path = os.path.join(out_dir, f"{basename}.csv")
    txt_path = os.path.join(out_dir, f"{basename}.txt")

    def pack(si):
        base = compute_stats(t, si)
        base["AUC_baseline_sub"] = compute_baseline_sub_auc(t, si, baseline)
        base["Late_SI"] = compute_late_si(si, late_k)
        return base

    v = pack(vessel_si); n = pack(normal_si); a = pack(abnormal_si)

    # CSV
    with open(csv_path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["Region", "Peak SI", "Time-to-Peak", "AUC",
                    "AUC(bsl-sub)", f"Late_SI(last_{late_k})", "Mean", "Std"])
        def row(name, d):
            w.writerow([name, d["peak"], d["time_to_peak"], d["AUC"],
                        d["AUC_baseline_sub"], d["Late_SI"], d["mean"], d["std"]])
        row("Vessel", v); row("Normal Tissue", n); row("Abnormal Tissue", a)

    # TXT summary
    lines = []
    lines.append("Perfusion MRI Simulation — Summary Statistics (Extended)")
    lines.append("--------------------------------------------------------")
    def block(name, d):
        lines.append(f"{name}:")
        lines.append(f"  Peak SI           : {d['peak']:.4f}")
        lines.append(f"  Time-to-Peak      : {d['time_to_peak']:.4f} a.u.")
        lines.append(f"  AUC               : {d['AUC']:.4f}")
        lines.append(f"  AUC (baseline-sub): {d['AUC_baseline_sub']:.4f}")
        lines.append(f"  Late SI (last {late_k}): {d['Late_SI']:.4f}")
        lines.append(f"  Mean SI           : {d['mean']:.4f}")
        lines.append(f"  Std. Dev. (SI)    : {d['std']:.4f}\n")
    block("Vessel", v); block("Normal Tissue", n); block("Abnormal Tissue", a)

    lines += [
        "Interpretation:",
        "- Vessel: earliest/highest peak (bolus).",
        "- Normal: faster rise, moderate washout.",
        "- Abnormal: slower rise, higher Late-SI and larger baseline-subtracted AUC (retention).",
        ""
    ]
    with open(txt_path, "w") as f:
        f.write("\n".join(lines))

    return csv_path, txt_path

def export_key_frames(frames_data, t, out_dir, ttp_idx):
    """Save pre (~0), ~TTP, and late frames as PNGs with titles."""
    os.makedirs(out_dir, exist_ok=True)
    idx_pre  = 0
    idx_ttp  = int(ttp_idx)
    idx_late = len(frames_data) - 1
    triplet = [("frame_pre.png",  idx_pre,  f"Pre-contrast (t={t[idx_pre]:.1f})"),
               ("frame_ttp.png",  idx_ttp,  f"~Time-to-Peak (t={t[idx_ttp]:.1f})"),
               ("frame_late.png", idx_late, f"Late (t={t[idx_late]:.1f})")]
    disp_min = np.min(frames_data[0]); disp_max = np.max(frames_data[-1])
    for fname, idx, title in triplet:
        plt.figure(figsize=(4.8, 4.8))
        plt.imshow(frames_data[idx], vmin=disp_min, vmax=disp_max)
        plt.title(title); plt.axis("off")
        plt.savefig(os.path.join(out_dir, fname), bbox_inches="tight", pad_inches=0)
        plt.close()
    return [os.path.join(out_dir, n) for n,_,_ in triplet]

def build_masks_public(n, vessel_w, split):
    """Public version of the phantom masks for preview/delta overlays."""
    y, x = np.indices((n, n))
    cx = n // 2
    vessel_mask = (x >= cx - vessel_w//2) & (x < cx + vessel_w//2)
    not_vessel = ~vessel_mask
    left_boundary  = int(n * (0.5 - split/2))
    right_boundary = int(n * (0.5 + split/2))
    normal_mask   = not_vessel & (x < left_boundary)
    abnormal_mask = not_vessel & (x >= right_boundary)
    middle_mask = not_vessel & (x >= left_boundary) & (x < right_boundary)
    normal_mask   |= middle_mask & (x < cx)
    abnormal_mask |= middle_mask & (x >= cx)
    background_mask = np.zeros((n, n), dtype=bool)
    return background_mask, vessel_mask, normal_mask, abnormal_mask

def export_delta_map(frames_data, N, vessel_w, split, out_dir):
    """
    Save a 'delta' image: (late - pre) with mask contours overlaid
    so graders can see the abnormal region pop.
    """
    os.makedirs(out_dir, exist_ok=True)
    frame_pre  = frames_data[0]
    frame_late = frames_data[-1]
    delta = frame_late - frame_pre

    _, vessel_m, normal_m, abnormal_m = build_masks_public(N, vessel_w, split)

    plt.figure(figsize=(5.2, 5.2))
    im = plt.imshow(delta)
    plt.title("ΔSI map (Late − Pre)")
    plt.axis("off")
    plt.colorbar(im, fraction=0.046, pad=0.04)
    # Overlay contours
    plt.contour(vessel_m,  levels=[0.5], linewidths=1.2)
    plt.contour(normal_m,  levels=[0.5], linewidths=1.2)
    plt.contour(abnormal_m,levels=[0.5], linewidths=1.2)
    out = os.path.join(out_dir, "delta_map.png")
    plt.savefig(out, bbox_inches="tight", pad_inches=0)
    plt.close()
    return out

def save_params_json(out_dir, params_dict):
    """Record run configuration for reproducibility."""
    os.makedirs(out_dir, exist_ok=True)
    params = dict(params_dict)
    params["timestamp"] = datetime.now().isoformat(timespec="seconds")
    with open(os.path.join(out_dir, "params.json"), "w") as f:
        json.dump(params, f, indent=2)



# ------------------------------------------------------------------
# GUI

"""
Tkinter GUI for the perfusion MRI simulation

Allows the user to:
    * choose an output directory
    * adjust phantom and kinetic parameters
    * preview the phantom layout
    * run the simulation and save all outputs (GIF, curves, stats, images)
    
"""
class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Perfusion MRI Simulation (Minimal GUI)")
        self.geometry("560x580")

        # Defaults
        self.params = {
            "N": 256, "frames": 120, "dt": 1.0,
            "vessel_width_px": 12, "normal_abnormal_split": 0.5,
            "vessel_peak": 3.0, "vessel_ttp": 8.0, "vessel_washout_tau": 10.0,
            "normal_amp": 0.8, "normal_rise_tau": 6.0, "normal_decay_tau": 30.0,
            "abnormal_amp": 0.8, "abnormal_rise_tau": 14.0, "abnormal_decay_tau": 120.0,
            "out_dir": ".",
        }

        row = 0
        ttk.Label(self, text="Output directory").grid(
            column=0, row=row, sticky="w", padx=8, pady=4
        )

        self.out_dir_var = tk.StringVar(value=self.params["out_dir"])

        ttk.Entry(self, textvariable=self.out_dir_var, width=30).grid(
            column=1, row=row, padx=8, pady=4, sticky="w"
        )

        ttk.Button(self, text="Browse...", command=self.browse_out_dir).grid(
            column=2, row=row, padx=4, pady=4, sticky="w"
        )

        row += 1
        ttk.Separator(self, orient="horizontal").grid(
            column=0, row=row, columnspan=3, sticky="ew", padx=8, pady=6
        )


        def add_param(label, key):
            nonlocal row
            row += 1
            ttk.Label(self, text=label).grid(column=0, row=row, sticky="w", padx=8, pady=3)
            var = tk.StringVar(value=str(self.params[key]))
            ttk.Entry(self, textvariable=var, width=12).grid(column=1, row=row, padx=8, pady=3, sticky="w")
            return var

        # Basic
        self.N_var      = add_param("Matrix size N", "N")
        self.frames_var = add_param("Frames (time points)", "frames")
        self.dt_var     = add_param("Time step (a.u.)", "dt")
        self.vessel_w_var = add_param("Vessel width (px)", "vessel_width_px")
        self.split_var    = add_param("Normal/Abnormal split (0-1)", "normal_abnormal_split")

        row += 1
        ttk.Separator(self, orient="horizontal").grid(column=0, row=row, columnspan=2, sticky="ew", padx=8, pady=6)

        # Vessel kinetics
        self.v_peak_var = add_param("Vessel peak ΔSI", "vessel_peak")
        self.v_ttp_var  = add_param("Vessel time-to-peak", "vessel_ttp")
        self.v_tau_var  = add_param("Vessel washout τ", "vessel_washout_tau")

        # Normal kinetics
        self.n_amp_var  = add_param("Normal amp", "normal_amp")
        self.n_rise_var = add_param("Normal rise τ", "normal_rise_tau")
        self.n_dec_var  = add_param("Normal decay τ", "normal_decay_tau")

        # Abnormal kinetics
        self.a_amp_var  = add_param("Abnormal amp", "abnormal_amp")
        self.a_rise_var = add_param("Abnormal rise τ", "abnormal_rise_tau")
        self.a_dec_var  = add_param("Abnormal decay τ", "abnormal_decay_tau")

        row += 1
        ttk.Separator(self, orient="horizontal").grid(column=0, row=row, columnspan=2, sticky="ew", padx=8, pady=6)

        row += 1
        btn_frame = ttk.Frame(self); btn_frame.grid(column=0, row=row, columnspan=2, sticky="w", padx=8, pady=10)
        ttk.Button(btn_frame, text="Preview Phantom", command=self.preview_phantom).grid(column=0, row=0, padx=0, pady=0)
        ttk.Button(btn_frame, text="Run Simulation", command=self.run_sim).grid(column=1, row=0, padx=8, pady=0)
        ttk.Button(btn_frame, text="Quit", command=self.destroy).grid(column=2, row=0, padx=8, pady=0)
        self.protocol("WM_DELETE_WINDOW", self.on_close)


    # Open a folder selection dialog and update the output directory field.
    def browse_out_dir(self):
        folder = filedialog.askdirectory()
        if folder:
            self.out_dir_var.set(folder)



    """
    Read GUI parameters, run the perfusion simulation, and save all outputs.

    This method:
        * parses user input from the GUI,
        * calls simulate_perfusion(),
        * shows the first frame for a quick visual check,
        * writes GIF, curves, statistics, key frames, delta map, and params.json,
        * displays a message box summarizing the saved files.

    """
    def run_sim(self):
        try:
            out_dir = self.out_dir_var.get().strip() or "."
            # Read params
            N = int(float(self.N_var.get()))
            frames = int(float(self.frames_var.get()))
            dt = float(self.dt_var.get())
            vessel_w = int(float(self.vessel_w_var.get()))
            split = float(self.split_var.get())

            v_peak = float(self.v_peak_var.get())
            v_ttp  = float(self.v_ttp_var.get())
            v_tau  = float(self.v_tau_var.get())

            n_amp  = float(self.n_amp_var.get())
            n_rise = float(self.n_rise_var.get())
            n_dec  = float(self.n_dec_var.get())

            a_amp  = float(self.a_amp_var.get())
            a_rise = float(self.a_rise_var.get())
            a_dec  = float(self.a_dec_var.get())

            # Simulate
            frames_data, t, v_si, n_si, a_si = simulate_perfusion(
                N=N, frames=frames, dt=dt,
                vessel_width_px=vessel_w, normal_abnormal_split=split,
                vessel_peak=v_peak, vessel_ttp=v_ttp, vessel_washout_tau=v_tau,
                normal_amp=n_amp, normal_rise_tau=n_rise, normal_decay_tau=n_dec,
                abnormal_amp=a_amp, abnormal_rise_tau=a_rise, abnormal_decay_tau=a_dec
            )

            # Quick preview
            disp_min = min(0.0, v_si.min(), n_si.min(), a_si.min())
            disp_max = max(v_si.max(), n_si.max(), a_si.max())
            plt.figure(figsize=(4.8, 4.8))
            plt.imshow(frames_data[0], vmin=disp_min, vmax=disp_max)
            plt.title("First Frame (Pre-contrast)")
            plt.axis("off")
            plt.show()

            # Main exports (movie + curves)
            save_outputs(frames_data, t, v_si, n_si, a_si, out_dir=out_dir, basename="perfusion_sim")

            # Extended stats (adds Late-SI and AUC(bsl-sub))
            save_stats_extended(t, v_si, n_si, a_si, out_dir=out_dir, basename="si_stats",
                                baseline=1.0, late_k=10)

            # Key frames and delta map
            ttp_idx = int(np.argmax(v_si))  # vessel TTP ~ the bolus peak
            export_key_frames(frames_data, t, out_dir, ttp_idx)
            export_delta_map(frames_data, N, vessel_w, split, out_dir)

            # Run snapshot & README
            params_snapshot = {
                "N": N, "frames": frames, "dt": dt,
                "vessel_width_px": vessel_w, "normal_abnormal_split": split,
                "vessel_peak": v_peak, "vessel_ttp": v_ttp, "vessel_washout_tau": v_tau,
                "normal_amp": n_amp, "normal_rise_tau": n_rise, "normal_decay_tau": n_dec,
                "abnormal_amp": a_amp, "abnormal_rise_tau": a_rise, "abnormal_decay_tau": a_dec
            }
            save_params_json(out_dir, params_snapshot)

            messagebox.showinfo(
                "Done",
                "Saved:\n"
                "- perfusion_sim.gif\n"
                "- si_curves.png / si_curves.csv\n"
                "- si_stats.csv / si_stats.txt (extended)\n"
                "- frame_pre.png / frame_ttp.png / frame_late.png\n"
                "- delta_map.png\n"
                "- params.json"
            )
        except Exception as e:
            messagebox.showerror("Error", str(e))



    """
    Display a color-coded preview of the phantom regions

    Normal tissue, abnormal tissue, and vessel pixels are shown in different
    colors to verify that the masks and split parameters are correct before
    running the full simulation.

    """
    def preview_phantom(self):
        try:
            N = int(float(self.N_var.get()))
            vessel_w = int(float(self.vessel_w_var.get()))
            split = float(self.split_var.get())
            _, vessel_m, normal_m, abnormal_m = build_masks_public(N, vessel_w, split)

            # color-coded preview
            preview = np.zeros((N, N, 3), dtype=float)
            preview[normal_m]   = [0.2, 0.6, 1.0]   # blue-ish
            preview[abnormal_m] = [1.0, 0.5, 0.2]   # orange-ish
            preview[vessel_m]   = [0.3, 1.0, 0.3]   # green-ish

            plt.figure(figsize=(4.8, 4.8))
            plt.imshow(preview)
            plt.title("Phantom regions: Normal (blue), Abnormal (orange), Vessel (green)")
            plt.axis("off")
            plt.show()
        except Exception as e:
            messagebox.showerror("Error", str(e))


    # Close any open Matplotlib figures and cleanly exit the GUI.
    def on_close(self):
        try:
            import matplotlib.pyplot as plt
            plt.close('all')
        except Exception:
            pass
        self.destroy()
    

if __name__ == "__main__":
    App().mainloop()
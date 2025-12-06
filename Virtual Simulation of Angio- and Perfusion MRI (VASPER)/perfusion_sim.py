"""
Perfusion MRI simulation - does not have the GUI

This script:
    * builds a simple 2D phantom with three compartments (vessel, normal tissue, abnormal tissue),
    * simulates their signal-intensity evolution over time using simple exponential models,
    * saves:
        * perfusion_sim.gif (or a sequence of PNG frames if GIF cannot be written)
        * si_curves.png (plot of SI vs time for each region)
        * si_curves.csv (numeric SI vs time data)

"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
import csv
import os


# USER TUNABLE PARAMETERS
N = 256                 # image size (NxN)
frames = 120            # number of time points
dt = 1.0                # time step (arbitrary units)
phantom_bg = 0.0        # background SI (project spec)
tissue_baseline = 1.0   # baseline tissue SI before contrast (project spec)
vessel_width_px = 12    # width of central vessel stripe
normal_abnormal_split = 0.5  # fraction of width labeled "normal" on left side

# Kinetic model params (simple exponentials)
# Vessel: sharp bolus then wash-out
vessel_peak = 3.0       # amplitude above baseline for vessel
vessel_ttp = 8.0        # time-to-peak (a.u.)
vessel_washout_tau = 10.0

# Normal tissue: faster in, moderate washout
normal_amp = 0.8
normal_rise_tau = 6.0
normal_decay_tau = 30.0

# Abnormal tissue: slower in, retains longer
abnormal_amp = 0.8
abnormal_rise_tau = 14.0
abnormal_decay_tau = 120.0  # long tau ~ retention

# Output paths (current folder)
gif_path = "perfusion_sim.gif"
curves_png_path = "si_curves.png"
curves_csv_path = "si_curves.csv"


# PHANTOM CONSTRUCTION
"""
The following function constructs boolean masks for background, vessel, normal, and abnormal tissue.

Parameters are: 
    n : int
        Matrix size (image is n x n).
    vessel_w : int
        Width in pixels of the central vessel stripe.
    split : float
        Fraction of the non-vessel width used to separate normal vs abnormal tissue.

        
Returns the following: 
    background_mask, vessel_mask, normal_mask, abnormal_mask : np.ndarray (bool)
        Boolean masks with the same shape (n x n), indicating each region.
    
    
Layout
    * A vertical vessel stripe is centered horizontally.
    * Remaining tissue is split into:
        * normal (left) and
        * abnormal (right),
    with a symmetric band around the vessel split half/half.
"""
def build_masks(n=256, vessel_w=12, split=0.5):
    y, x = np.indices((n, n))
    cx = n // 2
    # central vessel stripe
    vessel_mask = (x >= cx - vessel_w//2) & (x < cx + vessel_w//2)

    # everything not-vessel is tissue
    not_vessel = ~vessel_mask
    left_boundary  = int(n * (0.5 - split/2))
    right_boundary = int(n * (0.5 + split/2))

    normal_mask   = not_vessel & (x < left_boundary)
    abnormal_mask = not_vessel & (x >= right_boundary)

    # middle band (excluding vessel): left half = normal, right half = abnormal
    middle_mask = not_vessel & (x >= left_boundary) & (x < right_boundary)
    normal_mask   |= middle_mask & (x < cx)
    abnormal_mask |= middle_mask & (x >= cx)

    background_mask = np.zeros((n, n), dtype=bool)  # whole field = “body”
    return background_mask, vessel_mask, normal_mask, abnormal_mask

bg_mask, vessel_mask, normal_mask, abnormal_mask = build_masks(
    N, vessel_width_px, normal_abnormal_split
)


# KINETIC CURVES
t = np.arange(frames) * dt

# Vessel: quick rise ~ttp then exponential washout (gamma-variate-like simplification)
v_rise = np.exp(-np.maximum(0, vessel_ttp - t) / (vessel_ttp/3 + 1e-9))
v_wash = np.exp(-np.maximum(0, t - vessel_ttp) / (vessel_washout_tau + 1e-9))
vessel_delta = vessel_peak * v_rise * v_wash

# Normal tissue: rise * washout
normal_uptake  = (1 - np.exp(-t / (normal_rise_tau + 1e-9)))
normal_washout = np.exp(-t / (normal_decay_tau + 1e-9))
normal_delta   = normal_amp * (normal_uptake * normal_washout)

# Abnormal tissue: slower rise, slower decay (retention)
abn_uptake   = (1 - np.exp(-t / (abnormal_rise_tau + 1e-9)))
abn_washout  = np.exp(-t / (abnormal_decay_tau + 1e-9))
abnormal_delta = abnormal_amp * (abn_uptake * abn_washout)

# Total SI (baseline + delta)
vessel_si   = tissue_baseline + vessel_delta
normal_si   = tissue_baseline + normal_delta
abnormal_si = tissue_baseline + abnormal_delta


# FRAMES

# Builds a single NxN frame at time index i 
def frame_image_at_index(i):
    img = np.full((N, N), phantom_bg, dtype=float)
    img[normal_mask]   = normal_si[i]
    img[abnormal_mask] = abnormal_si[i]
    img[vessel_mask]   = vessel_si[i]
    return img

frames_data = [frame_image_at_index(i) for i in range(frames)]

disp_min = phantom_bg
disp_max = max(vessel_si.max(), normal_si.max(), abnormal_si.max())


# SAVE SI CURVES (PNG + CSV)
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

with open(curves_csv_path, "w", newline="") as f:
    w = csv.writer(f)
    w.writerow(["time", "vessel_si", "normal_si", "abnormal_si"])
    for i in range(len(t)):
        w.writerow([t[i], float(vessel_si[i]), float(normal_si[i]), float(abnormal_si[i])])


# ANIMATION (GIF)
fig = plt.figure(figsize=(4.8, 4.8))
im = plt.imshow(frames_data[0], vmin=disp_min, vmax=disp_max, animated=True)
plt.title("Perfusion MRI Simulation")
plt.axis("off")

def init():
    im.set_array(frames_data[0])
    return [im]

# Updates the function for the GIF animation
def animate(i):
    im.set_array(frames_data[i])
    return [im]

anim = animation.FuncAnimation(fig, animate, init_func=init, frames=frames, interval=80, blit=True)

try:
    from matplotlib.animation import PillowWriter
    anim.save(gif_path, writer=PillowWriter(fps=12))
except Exception:
    # If Pillow/PillowWriter isn’t available, save PNG frames instead
    os.makedirs("perfusion_frames", exist_ok=True)
    for i, img in enumerate(frames_data):
        plt.clf()
        plt.imshow(img, vmin=disp_min, vmax=disp_max)
        plt.axis("off")
        plt.title(f"Perfusion MRI Simulation (t={t[i]:.1f})")
        plt.savefig(f"perfusion_frames/frame_{i:04d}.png", bbox_inches="tight", pad_inches=0)

plt.close(fig)

print("Done. Generated files:")
print(" -", gif_path, "(or ./perfusion_frames/*.png if GIF writing not available)")
print(" -", curves_png_path)
print(" -", curves_csv_path)
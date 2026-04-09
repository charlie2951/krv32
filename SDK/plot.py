import serial
import serial.tools.list_ports
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
import re
import tkinter as tk
from tkinter import ttk, messagebox

# =====================================================
# Setup Window Logic
# =====================================================
class SetupWindow:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Sensor Plotter Setup")
        self.result = None

        # Port Selection
        tk.Label(self.root, text="Select COM Port:").grid(row=0, column=0, padx=10, pady=5)
        ports = [p.device for p in serial.tools.list_ports.comports()]
        self.port_var = tk.StringVar()
        self.port_cb = ttk.Combobox(self.root, textvariable=self.port_var, values=ports)
        self.port_cb.grid(row=0, column=1, padx=10, pady=5)
        if ports: self.port_cb.current(0)

        # Baud Rate
        tk.Label(self.root, text="Baud Rate:").grid(row=1, column=0)
        self.baud_var = tk.IntVar(value=115200)
        ttk.Combobox(self.root, textvariable=self.baud_var, values=[9600, 115200]).grid(row=1, column=1)

        # Number of Sensors
        tk.Label(self.root, text="Number of Sensors:").grid(row=2, column=0)
        self.num_sensors = tk.IntVar(value=3)
        ttk.Combobox(self.root, textvariable=self.num_sensors, values=[1, 2, 3]).grid(row=2, column=1)

        # Labels for Graph and Search String
        tk.Label(self.root, text="Graph Label (Y-Axis)").grid(row=3, column=1)
        tk.Label(self.root, text="Search Prefix (e.g. Sensor1:)").grid(row=3, column=2)

        self.name_entries = []
        self.prefix_entries = []
        
        defaults = [("Temp (°C)", "Temperature:"), ("Press (hPa)", "Pressure:"), ("Hum (%)", "Humidity:")]
        
        for i in range(3):
            tk.Label(self.root, text=f"Sensor {i+1}:").grid(row=4+i, column=0)
            
            # Display Name
            ne = tk.Entry(self.root)
            ne.insert(0, defaults[i][0])
            ne.grid(row=4+i, column=1, padx=5, pady=2)
            self.name_entries.append(ne)
            
            # Search Prefix
            pe = tk.Entry(self.root)
            pe.insert(0, defaults[i][1])
            pe.grid(row=4+i, column=2, padx=5, pady=2)
            self.prefix_entries.append(pe)

        tk.Button(self.root, text="Start Plotting", command=self.on_submit, bg="#4CAF50", fg="white").grid(row=7, columnspan=3, pady=15)

    def on_submit(self):
        self.result = {
            "port": self.port_var.get(),
            "baud": self.baud_var.get(),
            "count": self.num_sensors.get(),
            "names": [n.get() for n in self.name_entries],
            "prefixes": [p.get() for p in self.prefix_entries]
        }
        self.root.destroy()

    def run(self):
        self.root.mainloop()
        return self.result

setup = SetupWindow()
config = setup.run()
if not config: exit()

# =====================================================
# Plotting Logic
# =====================================================
NUM_SENSORS = config["count"]
SENSOR_NAMES = config["names"]
PREFIXES = config["prefixes"]
WINDOW_SIZE = 40

data_buffers = [deque(maxlen=WINDOW_SIZE) for _ in range(NUM_SENSORS)]
ser = serial.Serial(config["port"], config["baud"], timeout=0.1)

fig, axes = plt.subplots(NUM_SENSORS, 1, sharex=True, figsize=(9, 3*NUM_SENSORS))
if NUM_SENSORS == 1: axes = [axes]
fig.canvas.manager.set_window_title('General Sensor Monitor')

lines = []
colors = ['#FF5733', '#3357FF', '#33FF57']

for i in range(NUM_SENSORS):
    line, = axes[i].plot([], [], 'o-', color=colors[i], markersize=4, linewidth=1.5, label=SENSOR_NAMES[i])
    axes[i].set_ylabel(SENSOR_NAMES[i])
    axes[i].grid(True, linestyle='--', alpha=0.6)
    axes[i].legend(loc='upper left')
    lines.append(line)

def init():
    for ax in axes:
        ax.set_xlim(0, WINDOW_SIZE)
    return lines

def update(frame):
    if ser.in_waiting > 0:
        try:
            line_str = ser.readline().decode('utf-8').strip()
            
            for i in range(NUM_SENSORS):
                # Search for the specific prefix + a number
                # Example: If prefix is "Sensor1:", it looks for "Sensor1:\s*([\d.-]+)"
                pattern = re.escape(PREFIXES[i]) + r"\s*([-+]?\d*\.\d+|\d+)"
                match = re.search(pattern, line_str)
                
                if match:
                    val = float(match.group(1))
                    data_buffers[i].append(val)
                    lines[i].set_data(range(len(data_buffers[i])), list(data_buffers[i]))
                    
                    # Auto-scaling logic
                    if len(data_buffers[i]) > 1:
                        v_min, v_max = min(data_buffers[i]), max(data_buffers[i])
                        padding = (v_max - v_min) * 0.1 if v_max != v_min else 1.0
                        axes[i].set_ylim(v_min - padding, v_max + padding)
                        
        except Exception:
            pass 

    return lines

ani = animation.FuncAnimation(fig, update, init_func=init, interval=100, blit=False)
plt.tight_layout()
plt.show()
ser.close()
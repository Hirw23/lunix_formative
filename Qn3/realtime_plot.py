# realtime_plot_simple.py
import simulator
import time
from datetime import datetime

def ts_to_str(ts):
    return datetime.fromtimestamp(ts).strftime('%H:%M:%S')

def main():
    duration = 30.0  # seconds
    interval = 2.0   # seconds
    sim = simulator.Simulator(duration, interval)

    print("Time     | Temperature (°C) | Humidity (%) | Battery (%)")
    print("---------------------------------------------------------")

    try:
        for reading in sim:
            ts, temp, hum, bat = reading
            print(f"{ts_to_str(ts)} |     {temp:6.2f}     |   {hum:6.2f}  |   {bat:6.2f}")
    except KeyboardInterrupt:
        print("\nSimulation interrupted by user.")
    print("Simulation finished.")

if __name__ == "__main__":
    main()


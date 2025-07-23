import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Datenpuffer
ax_data, ay_data, az_data = [], [], []
gx_data, gy_data, gz_data = [], [], []

# Subplots definieren
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, sharex=True)
fig.suptitle("MPU9250: Acceleration & Gyroscope")

def update(frame):
    try:
        with open("data/mpu9250_log.txt") as f:
            lines = f.readlines()[-200:]  # use the last x lines

        print(f"{len(lines)} Zeilen gelesen")  # Debug-Output

        # Listen leeren
        ax_data.clear(); ay_data.clear(); az_data.clear()
        gx_data.clear(); gy_data.clear(); gz_data.clear()

        for line in lines:
            try:
                ax, ay, az, gx, gy, gz = map(float, line.strip().split())
                ax_data.append(ax)
                ay_data.append(ay)
                az_data.append(az)
                gx_data.append(gx)
                gy_data.append(gy)
                gz_data.append(gz)

            except ValueError:
                print("Fehler beim Parsen:", line.strip())
                continue
    except Exception as e:
        print("Fehler beim Öffnen der Datei:", e)
        return
    
   # Achsen neu zeichnen
    ax1.cla(); ax2.cla(); ax3.cla()

    ax1.plot(ax_data, label='ax', color='tab:blue')
    ax1.plot(ay_data, label='ay', color='tab:orange')
    ax1.plot(az_data, label='az', color='tab:green')
    ax1.set_ylabel("Beschl. [m/s²]")
    ax1.set_ylim(-20, 20)
    ax1.legend(loc="upper right")
    ax1.grid(True)

    ax2.plot(gx_data, label='gx', color='tab:red')
    ax2.plot(gy_data, label='gy', color='tab:purple')
    ax2.plot(gz_data, label='gz', color='tab:brown')
    ax2.set_ylabel("Gyro [°/s]")
    ax2.set_ylim(-500, 500)
    ax2.legend(loc="upper right")
    ax2.grid(True)

    ax3.plot(az_data, label='az (vertikal)', color='tab:gray')
    ax3.set_ylabel("Z-Beschl. [m/s²]")
    ax3.set_ylim(-10, 10)
    ax3.legend(loc="upper right")
    ax3.grid(True)
    ax3.set_xlabel("Zeit (Samples)")

ani = animation.FuncAnimation(plt.gcf(), update, interval=200)
plt.tight_layout()
plt.show()

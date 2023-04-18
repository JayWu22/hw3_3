import struct
import serial
import matplotlib.pyplot as plt
import time
import numpy as np


# Set up serial connection
ser = serial.Serial('COM5')  # Change 'COM3' to the port your mbed is connected to

# Send start command to mbed
x = np.uint8(0X10)
output = x.tobytes()
print(output)
ser.write(output)

# Read accelerometer values from mbed
accel_datax = []
accel_datay = []
accel_dataz = []


for i in range(128):
    datax = ser.read(8)  # Read 24 bytes (3 doubles) at a time
    time.sleep(0.1)
    print(datax)
    datay = ser.read(8)
    time.sleep(0.1)
    dataz = ser.read(8)
    time.sleep(0.1)
    accel_datax.append(struct.unpack('d', datax))
    accel_datay.append(struct.unpack('d', datay))
    accel_dataz.append(struct.unpack('d', dataz))



# Plot accelerometer values
fig, (ax1, ax2, ax3) = plt.subplots(3, 1)
fig.suptitle('Accelerometer Data')
ax1.plot([i*10 for i in range(128)], accel_datax)
ax1.set_ylabel('X')
ax2.plot([i*10 for i in range(128)], accel_datay)
ax2.set_ylabel('Y')
ax3.plot([i*10 for i in range(128)], accel_dataz)
ax3.set_ylabel('Z')
ax3.set_xlabel('Time (ms)')

plt.show()

# Close serial connection
ser.close()

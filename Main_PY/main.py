import serial
import serial.tools.list_ports
import time
import struct


def send_velocity(vx, vy, ser):
    # Prepend the command byte for velocity data
    command_byte = b'\xBB'
    velocity_data = struct.pack('<ff', vx, vy)  # Pack vx and vy as floats
    # Combine command byte with velocity data
    message = command_byte + velocity_data
    # Send the combined message using the existing connection
    ser.write(message)


teensy_port = 'COM11'  # Placeholder - replace with your actual Teensy port
firebeetle_port = 'COM17'
ser2 = serial.Serial(teensy_port, 115200, timeout=1)


def read_joystick_values():
    # Check if more than 8 bytes are waiting, keep only the latest 8 bytes
    while ser2.in_waiting > 8:
        # Read and discard all but the last 8 bytes
        ser2.read(ser2.in_waiting - 8)

    if ser2.in_waiting == 8:
        data = ser2.read(8)
        vx, vy = struct.unpack('<ff', data)
        return vx, vy
    else:
        return None, None



if firebeetle_port and teensy_port:
    # Open serial connection once
    ser = serial.Serial(firebeetle_port, 115200, timeout=1)
    print(f"ESP8266 found at {firebeetle_port}")
    print(f"Teensy found at {teensy_port}")
    while True:
        vx, vy = read_joystick_values()
        if vx is not None and vy is not None:  # Valid data received
            print(f"Joystick X: {vx}, Y: {vy}")  # Print the joystick values
            send_velocity(vx, vy, ser)  # Use the persistent serial connection
            time.sleep(0.2)
else:
    print("Device not found or not connected")

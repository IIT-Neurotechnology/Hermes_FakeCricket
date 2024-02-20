import serial
import serial.tools.list_ports
import time
import struct

def find_firebeetle_port():
    ports = list(serial.tools.list_ports.comports())
    for port in ports:
        try:
            ser = serial.Serial(port.device, 115200, timeout=1)
            time.sleep(1)  # Wait for the connection to establish
            ser.write(b'\xAA')  # Send the byte command for identification
            ser.flush()  # Ensure the command is sent immediately
            time.sleep(0.1)  # Short delay to allow response
            if ser.in_waiting:
                response = ser.read()
                if response == b'\xAA':  # Check if the response matches the expected byte
                    ser.close()
                    return port.device
            ser.close()
        except (OSError, serial.SerialException):
            pass
    return None

def send_velocity(vx, vy, ser):
    # Prepend the command byte for velocity data
    command_byte = b'\xBB'
    velocity_data = struct.pack('<ff', vx, vy)  # Pack vx and vy as floats
    message = command_byte + velocity_data  # Combine command byte with velocity data
    ser.write(message)  # Send the combined message using the existing connection
    
teensy_port = 'COM11'  # Placeholder - replace with your actual Teensy port
firebeetle_port = find_firebeetle_port()
ser2=serial.Serial(teensy_port, 115200, timeout=1)

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
    ser = serial.Serial(firebeetle_port, 115200, timeout=1)  # Open serial connection once
    print(f"ESP8266 found at {firebeetle_port}")
    print(f"Teensy found at {teensy_port}")
    while True:
        vx, vy = read_joystick_values()
        if vx is not None and vy is not None:  # Valid data received
            print(f"Joystick X: {vx}, Y: {vy}")  # Print the joystick values
            send_velocity(vx, vy, ser)  # Use the persistent serial connection
else:
    print("Device not found or not connected")

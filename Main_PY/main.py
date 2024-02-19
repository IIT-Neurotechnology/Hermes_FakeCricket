import serial
import serial.tools.list_ports
import time
import struct
        
def find_firebeetle_port():
    ports = list(serial.tools.list_ports.comports())
    for port in ports:
        try:
            ser = serial.Serial(port.device, 115200, timeout=1)
            time.sleep(2)  # Wait for the connection to establish
            ser.write(b'\xAA')  # Send the byte command for identification
            ser.flush()  # Ensure the command is sent immediately
            time.sleep(0.1)  # Short delay to allow response
            if ser.in_waiting:
                response = ser.read()
                if response == b'\xBB':  # Check if the response matches the expected byte
                    ser.close()
                    return port.device
            ser.close()
        except (OSError, serial.SerialException):
            pass
    return None

def send_velocity(vx, vy, port):
    with serial.Serial(port, 115200, timeout=1) as ser:
        time.sleep(2)  # wait for the serial connection to initialize
        message = struct.pack('<ff', vx, vy)  # Pack vx and vy as floats
        ser.write(message)  # Send the packed bytes

firebeetle_port = find_firebeetle_port()
if firebeetle_port:
    print(f"Server found at {firebeetle_port}")
else:
    print("Server not found or not Wi-Fi connected")
    
vx = 1.0  # Example velocity x
vy = 1.0  # Example velocity y
send_velocity(vx, vy, firebeetle_port)
import serial
import serial.tools.list_ports
import time
import struct

def send_velocity(vx, vy, w,ser):
    # Prepend the command byte for velocity data
    start_byte = b'\xAA'
    end_byte = b'\x55'
    velocity_data = struct.pack('<ff', vx, vy, w)  # Pack vx and vy and w as floats
    # Combine command byte with velocity data
    message = start_byte + velocity_data + end_byte
    # Send the combined message using the existing connection
    ser.write(message)


teensy_port = 'COM28'  # Placeholder - replace with your actual Teensy port
firebeetle_port = 'COM29'
ser2 = serial.Serial(teensy_port, 115200, timeout=1)


def read_joystick_values():
    start_byte_found = False
    message_buffer = bytearray()

    # Discard older bytes if more than 20 bytes are in the buffer.
    if ser2.in_waiting > 20:
        ser2.read(ser2.in_waiting - 20)

    # Read through the available data to find the latest complete packet.
    while ser2.in_waiting:
        byte = ser2.read(1)
        if not start_byte_found:
            # Look for the start byte
            if byte == b'\xAA':
                start_byte_found = True
                message_buffer = bytearray()  # Reset buffer upon finding start byte
        else:
            # Start byte found, now reading message until end byte
            if byte == b'\x55':  # End byte found
                if len(message_buffer) == 12:
                    # Ensure we have exactly 12 bytes for three floats (vx, vy, w)
                    vx, vy, w = struct.unpack('<ff', message_buffer)
                    return vx, vy , w
                # If the packet is not exactly 8 bytes, reset and continue looking.
                start_byte_found = False
                message_buffer = bytearray()
            else:
                # Accumulate the message bytes
                message_buffer.append(byte[0])

    # If exiting the loop without finding a complete packet, return None.
    return None, None




if firebeetle_port and teensy_port:
    # Open serial connection once
    ser = serial.Serial(firebeetle_port, 115200, timeout=1)
    print(f"ESP8266 found at {firebeetle_port}")
    print(f"Teensy found at {teensy_port}")
    while True:
        w=0
        vx, vy = read_joystick_values()
        if vx is not None and vy is not None:  # Valid data received
            print(f"Joystick X: {vx}, Y: {vy}, W:{w}")  # Print the joystick values
            send_velocity(vx, -vy, w, ser)  # Use the persistent serial connection
            #time.sleep(0.1)
else:
    print("Device not found or not connected")

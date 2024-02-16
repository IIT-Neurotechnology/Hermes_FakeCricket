import serial

def send_velocity(ser,values):
    
    if ser.isOpen():
        vel_str = ' '.join(str(v) for v in values)
        vel_str+='\n'
        ser.write(vel_str.encode())
        print(vel_str)

    else: print('Serial port is closed')

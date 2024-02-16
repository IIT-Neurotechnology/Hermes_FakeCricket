import keyboard
import numpy as np
from Inv_Kinematic import Kinematic
from Inv_Kinematic import Kinematic_test
import serial
from Wireless import send_velocity
import time

def manual_movement(ser):
    print("Manual movement mode: use WASD for movement, and Q to quit.")

    while True:
        velocity = [0, 0, 0]
        velocity = Kinematic(velocity)
        send_velocity(ser, velocity)
        time.sleep(0.2)
        if keyboard.is_pressed('w'):
            velocity = [0, 150, 0]  # Modify this to correspond to the robot's movement
            velocity = Kinematic(velocity)
            send_velocity(ser, velocity)
            time.sleep(1)
        elif keyboard.is_pressed('a'):
            velocity = [-150, 0, 0]
            velocity = Kinematic(velocity)
            send_velocity(ser, velocity)
            time.sleep(1)
            
        elif keyboard.is_pressed('s'):
            velocity = [0,-150, 0]
            velocity = Kinematic(velocity)
            send_velocity(ser, velocity)
            time.sleep(1)
        elif keyboard.is_pressed('d'):
            velocity = [150, 0, 0]
            velocity = Kinematic(velocity)
            send_velocity(ser, velocity)
            time.sleep(1)
        elif keyboard.is_pressed('e'):
            velocity = [150, 150, 0]
            velocity = Kinematic(velocity)
            send_velocity(ser, velocity)
            time.sleep(1)
        elif keyboard.is_pressed('q'):
            velocity = [-150, 150, 0]
            velocity = Kinematic(velocity)
            send_velocity(ser, velocity)
            time.sleep(1)
        elif keyboard.is_pressed('c'):
            velocity = [150, -150, 0]
            velocity = Kinematic(velocity)
            send_velocity(ser, velocity)
            time.sleep(1)


            
        elif keyboard.is_pressed('k'):  # Quit
            print("Exiting manual mode...")
            break
        
def manual_movement_test(ser):
    print("Manual movement mode: use 1-3 to select motor, F/B to choose direction, S to set speed, and Q to quit.")

    motor = 0
    speed = 0
    direction = 'F'

    while True:
        velocity = [0, 0, 0]
        time.sleep(0.2)
        
        # Add a way to select motor and set direction and speed.
        if keyboard.is_pressed('1'):
            motor = 0
            print("Selected motor 1")
        elif keyboard.is_pressed('2'):
            motor = 1
            print("Selected motor 2")
        elif keyboard.is_pressed('3'):
            motor = 2
            print("Selected motor 3")
        elif keyboard.is_pressed('f'):
            direction = 'F'
            print("Selected forward direction")
        elif keyboard.is_pressed('b'):
            direction = 'B'
            print("Selected backward direction")
        elif keyboard.is_pressed('s'):
            speed = input("Enter speed: ")
            print(f"Speed set to {speed}")

        # Modify velocity according to selected motor, speed and direction
        if direction == 'F':
            velocity[motor] = int(speed)
        else:
            velocity[motor] = -int(speed)
        
        velocity=Kinematic_test(velocity)
        send_velocity(ser, velocity)
        time.sleep(0.1)

        # Quit option
        if keyboard.is_pressed('q'):  
            print("Exiting manual mode...")
            break

def joystick(ser2,ser):
        while True:
            if ser2.in_waiting == 0: # Check if there's data waiting to be read
                time.sleep(0.1)
            else:
                line = ser2.readline().decode('utf-8').strip()
                parts = line.split(',')
                if len(parts) >=2:
                    try:
                        x = int(float((parts[0].split(':')[1].strip())))
                        y = int(float((parts[1].split(':')[1].strip())))
                        print("A12 value: ", x)
                        print("A13 value: ", y)
                        velocity=[x, y, 0]
                        ser2.reset_input_buffer()
                        velocity=(Kinematic(velocity))
                        send_velocity(ser, velocity)
                    except ValueError:
                        print("Could not convert string to float. Raw line: ", line)
                else:
                    print("Not enough data was read from the serial port. Raw line: ", line)
                    ser2.reset_input_buffer()
            if keyboard.is_pressed('q'):  
                    print("Exiting manual mode...")
                    break

        ser2.close()
        ser.close()
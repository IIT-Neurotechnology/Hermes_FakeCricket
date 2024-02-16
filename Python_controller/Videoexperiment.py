import serial
import Basler
import Manualcontrol
import saved_video

try:
    ser = serial.Serial('COM17', 115200, timeout=1)
except serial.SerialException:
    print("There is no Wi-fi Communication")

try:
    ser2 = serial.Serial('COM11', 9600, timeout=1)
except serial.SerialException:
    print("There is no Joystick")


mode = input("Enter 'm' for manual control, or 'a' for automatic control, t for test j for joystick , v for vengace: ")
if mode == 'm':
    Manualcontrol.manual_movement(ser)
elif mode == 't':
    Manualcontrol.manual_movement_test(ser)
elif mode == 'j':
    Manualcontrol.joystick(ser2, ser)
elif mode == 'a':
    saved_video.analysis('Video1.mp4', ser)
elif mode == 'v':
    Basler.cam_experiment(ser)


else:
    print("Invalid mode entered. Please enter either 'manual' or 'auto'.")
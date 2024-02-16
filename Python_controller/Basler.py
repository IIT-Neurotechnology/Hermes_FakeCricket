from __future__ import print_function
from pypylon import pylon
from pypylon import genicam
from undistort import undistort_image
import wallforce
import Orientation
from Wireless import send_velocity
from Inv_Kinematic import Kinematic
import numpy as np
import time
import Filters
import cv2
import keyboard
import threading

KF = Filters.Kalman(0.1, 1, 1, 1, 0.1,0.1)
cricket_tracker = Orientation.CricketTracker(frame_window=10)
global send_velocity_flag
global plotFF
plotFF=0

def cam_experiment(ser):
    try:
        imageWindow = pylon.PylonImageWindow()
        imageWindow.Create(1)
        camera = pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice())
        print("Using device ", camera.GetDeviceInfo().GetModelName())
    except genicam.GenericException as e:
        print("An exception occurred.")
        print(e.GetDescription())
    camera.StartGrabbingMax(100000, pylon.GrabStrategy_LatestImageOnly)
    mask,x_force,y_force,xwall,ywall,w,h,img_ph=grabROI(camera)
    #layer_threshold,layer_full,counter=GUI._init_(img_ph)
    converter = pylon.ImageFormatConverter()
    converter.OutputPixelFormat = pylon.PixelType_BGR8packed
    converter.OutputBitAlignment = pylon.OutputBitAlignment_MsbAligned
    cv2.namedWindow('img_tresh', cv2.WINDOW_NORMAL)
    cv2.namedWindow('gray', cv2.WINDOW_NORMAL)
    cv2.createTrackbar('Threshold', 'img_tresh', 10, 255, nothing) # Initial threshold value is 0, max is 255
    send_velocity_flag=True
    while camera.IsGrabbing():
        key = cv2.waitKey(1) & 0xFF
        if key == ord('s'):
            send_velocity_flag = not send_velocity_flag
        grabResult = camera.RetrieveResult(5000, pylon.TimeoutHandling_ThrowException)
        if grabResult.GrabSucceeded():
            image = converter.Convert(grabResult)
            img = image.GetArray()
            centers,img,img_thresh=detect(img,1,mask)
            # If centroids are detected then track them
            if (len(centers) > 0):
                sr_coord_x,sr_coord_y,x,y=filterandassign(centers,xwall,ywall,w,h,img)
                wall_force = [int(x_force[sr_coord_y,sr_coord_x]),int(y_force[sr_coord_y,sr_coord_x])]
                if send_velocity_flag:
                    velocity = np.array(wall_force)
                    print(velocity,"\n")
                    print( sr_coord_x,sr_coord_y)
                else:
                    velocity = np.array([0,0])
                #summed_position_vector, summed_velocity_vector = cricket_tracker.update_cricket(np.array([x, y]), velocity)
                rot_matrix=cricket_tracker.rotation_matrix()
                velocity_rot = np.dot(rot_matrix, velocity)
                velocity_rot = np.append(velocity_rot, 0)
                velocity_rot[0] = -velocity_rot[0]
                print(velocity_rot,"\n")
                velocity_motors = Kinematic(velocity_rot)
                send_velocity(ser, velocity_motors)
                #GUI.Nviewer(layer_threshold,layer_full,img,img_thresh,counter)
                cv2.imshow('gray', img)
            k = cv2.waitKey(1)
            if k == 27:
                grabResult.Release()
                break
        else:
            # grabResult.ErrorDescription does not work properly in python could throw UnicodeDecodeError
            print("Error: ", grabResult.ErrorCode)
            grabResult.Release()
            time.sleep(0.05)


    camera.Close()
    imageWindow.Close()

def detect(img,debugMode,mask):
    font = cv2.FONT_HERSHEY_SIMPLEX
    fontScale = 0.5
    color = (100, 50, 50)
    thickness = 1
    img=undistort_image(img)
    threshold_value = cv2.getTrackbarPos('Threshold', 'img_tresh') # Get current position of trackbar
    # Convert frame from BGR to GRAY
    img = cv2.bitwise_and(img, img, mask=mask)
    assert img is not None, "file could not be read, check with os.path.exists()"

    ret, img_thresh = cv2.threshold(img, threshold_value, 255, cv2.THRESH_BINARY)
    img_thresh = cv2.cvtColor(img_thresh, cv2.COLOR_BGR2GRAY)


    # Find contours
    contours, hieararchy = cv2.findContours(img_thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    i = 0
    min_radius_thresh= 15
    max_radius_thresh= 30   
    centers=[]
    for c in contours:
        (x, y), radius = cv2.minEnclosingCircle(c)
        radius = int(radius)

        #Take only the valid circles
        if (radius > min_radius_thresh) and (radius < max_radius_thresh):
            centers.append(np.array([[x], [y]]))
            img_thresh = cv2.putText(img_thresh, 'Cricket', (int(centers[0][0]),int(centers[0][1])), font,fontScale, color, thickness, cv2.LINE_AA)
            ellipse = cv2.fitEllipse(c)
            cv2.ellipse(img_thresh, ellipse, (50, 255, 255), 3)
            cv2.imshow('img_tresh', img_thresh)
            
    return centers,img,img_thresh

def nothing(x):
    pass

def grabROI(camera):
    cv2.namedWindow("Select ROI", cv2.WINDOW_NORMAL)
    # Select region of interest, undistort, create wallforce return image cropped 
    grab_result = camera.RetrieveResult(5000, pylon.TimeoutHandling_ThrowException)
    if grab_result.GrabSucceeded():
        img = grab_result.Array
        img=undistort_image(img)
        roi = cv2.selectROI("Select ROI", img)
        cv2.destroyWindow("Select ROI")

        # Set region of interest as image mask
        xwall, ywall, w, h = roi
        mask = np.zeros((camera.Height.Value, camera.Width.Value), dtype=np.uint8)
        mask[ywall:ywall+h, xwall:xwall+w] = 255
        x_force, y_force = wallforce.calculate_force_field(w,h,0.0000001)
        wallforce.plot_force_field(x_force, y_force, plotFF)
    else:
        raise RuntimeError("Failed to grab initial frame.")
    return mask,x_force,y_force,xwall,ywall,w,h,img

def filterandassign(centers,xwall,ywall,w,h,img):
    cv2.circle(img, (int(centers[0][0]), int(centers[0][1])), 10, (0, 191, 255), 2)
    (x, y) = KF.predict()
    (x1, y1) = KF.update(centers[0])
    cv2.rectangle(img, (int(x - 15), int(y - 15)), (int(x + 15), int(y + 15)), (255, 0, 0), 2)
    cv2.rectangle(img, (int(x1 - 15), int(y1 - 15)), (int(x1 + 15), int(y1 + 15)), (0, 0, 255), 2)
    cv2.putText(img, "Estimated Position", (int(x1 + 15), int(y1 + 10)), 0, 0.5, (0, 0, 255), 2)
    cv2.putText(img, "Predicted Position", (int(x + 15), int(y)), 0, 0.5, (255, 0, 0), 2)
    cv2.putText(img, "Measured Position", (int(centers[0][0] + 15), int(centers[0][1] - 15)), 0, 0.5, (0,191,255), 2)
    sr_coord_x=np.clip(int(x)-xwall,1,w-1)
    sr_coord_y=np.clip(int(y)-ywall,1,h-1)
    return sr_coord_x,sr_coord_y,x,y

def Calibration_Movement(ser,camera):
    print("Press Space to start calibration")
    keyboard.wait('space')
    grabResult = camera.RetrieveResult(5000, pylon.TimeoutHandling_ThrowException)

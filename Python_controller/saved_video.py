import cv2 as cv
import numpy as np
import time
from pypylon import pylon, genicam  # Assuming you are using pylon and genicam packages
from undistort import undistort_image
from Wireless import send_velocity
from Inv_Kinematic import Kinematic
from Inv_Kinematic import Kinematic_test
from MouseForce import rat_repulsion_force


def analysis(video_path, ser):
    try:
        vidcap = cv.VideoCapture(video_path)
        success, img = vidcap.read()
        if success == 1:
            img = undistort_image(img)
            cv.namedWindow('Select ROI', cv.WINDOW_NORMAL)
            cv.resizeWindow("Select ROI", 1020, 780)
            roi = cv.selectROI("Select ROI", img)
            cv.destroyWindow("Select ROI")

            x1, y1, w1, h1 = roi

        else:
            raise RuntimeError("Failed to grab initial frame.")
        
        font = cv.FONT_HERSHEY_SIMPLEX
        fontScale = 0.5
        color = (100, 50, 50)
        thickness = 1

        cv.namedWindow('Contours', cv.WINDOW_NORMAL)
        cv.resizeWindow("Contours", 1020, 780)
        while success:
            success, img = vidcap.read()
            ret, img = cv.threshold(img, 11, 255, cv.THRESH_BINARY)
            img = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
            img = crop_image(img, x1, y1, w1, h1)
            contours, h = cv.findContours(img, cv.RETR_LIST, cv.CHAIN_APPROX_SIMPLE)
            for contour in contours:
                area = cv.contourArea(contour)
                if (area < 500) or (area > 10000):
                    continue
                x, y, w, h = cv.boundingRect(contour)
                cx = x + w // 2
                cy = y + h // 2
                if (area < 900):
                    cxk = cx
                    cyk = cy
                    cv.circle(img, (cxk, cyk), 2, (50, 255, 255), 2)
                    img = cv.putText(img, 'Cricket', (x, y), font, fontScale, color, thickness, cv.LINE_AA)
                    ellipse = cv.fitEllipse(contour)
                    cv.ellipse(img, ellipse, (50, 255, 255), 3)
                elif area > 3000:
                    rat_center = (cx, cy)
                    cv.circle(img, (cx, cy), 2, (50, 255, 255), 2)
                    img = cv.putText(img, 'Rat', (x, y), font, fontScale, color, thickness, cv.LINE_AA)
                    ellipse = cv.fitEllipse(contour)
                    cv.ellipse(img, ellipse, (50, 255, 255), 3)
                else:
                    continue

            rat_force = rat_repulsion_force((cxk, cyk), rat_center)
            total_force = rat_force
            velocity = np.resize(total_force, 3)
            velocity = Kinematic(velocity)
            send_velocity(ser, velocity)
            time.sleep(0.3)
            cv.imshow('Contours', img)
            k = cv.waitKey(1)
            if k == 27:
                break
    except genicam.GenericException as e:
        print("An exception occurred.")
        print(e.GetDescription())

def crop_image(image, x, y, width, height):
    return image[y:y + height, x:x + width]
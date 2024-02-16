import napari
import cv2
import numpy as np
import time

def Nviewer(layer_threshold,layer_full,img_threshold,img_full,counter):
    if counter % 10 == 0:
        layer_threshold.data = img_threshold
        layer_full.data = img_full
        #napari.run()  # or napari.qt.thread_worker depending on napari version
    counter += 1
    return counter
    
def _init_(img_ph):
    counter=0
    viewer = napari.Viewer()
    layer_threshold = viewer.add_image(np.zeros_like(img_ph))  # placeholder
    layer_full = viewer.add_image(np.zeros_like(img_ph))  # placeholder
    return layer_threshold,layer_full,counter
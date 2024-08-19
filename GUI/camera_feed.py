import pyrealsense2 as rs
import numpy as np
from PIL import Image, ImageOps
import customtkinter
import cv2

class CameraFeed:
    def __init__(self, label):
        self.label = label
        self.pipeline = rs.pipeline()
        self.config = rs.config()
        self.config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
        self.config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
        self.pipeline.start(self.config)
        self.imgtk = None

    def update(self):
        frames = self.pipeline.wait_for_frames()
        color_frame = frames.get_color_frame()
        depth_frame = frames.get_depth_frame()
        if not depth_frame or not color_frame:
            return

        # Convert images to numpy arrays
        depth_image = np.asanyarray(depth_frame.get_data())
        color_image = np.asanyarray(color_frame.get_data())

        # Apply colormap on depth image (image must be converted to 8-bit per pixel first)
        depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.03), cv2.COLORMAP_JET)

        # Stack both images horizontally
        images = np.hstack((color_image, depth_colormap))

        self.imgtk = customtkinter.CTkImage(light_image=color_image, dark_image=color_image, size=(self.label.winfo_width(), self.label.winfo_height()))
        self.label.image = self.imgtk  # Ensure reference is kept
        self.label.configure(image=self.imgtk)


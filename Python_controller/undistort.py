import cv2
import numpy as np

def undistort_image(img):
    # Define camera matrix and distortion coefficients
    K = np.array([[1753.1764271270376, 0, 769.7307347759404],
                  [0, 1753.4389795897814, 549.32740999568955],
                  [0, 0, 1]])
    dist_coeffs = np.array([-0.26616998352506377,0.14393593599317869,0.00020055073869918146,0.00041759849013703147,-0.10760272994714998],)
    
    # Undistort image
    undistorted = cv2.undistort(img, K, dist_coeffs, None, K)
    
    return undistorted

def undistort_points(points):
    # Define camera matrix and distortion coefficients
    K = np.array([[1753.1764271270376, 0, 769.7307347759404],
                  [0, 1753.4389795897814, 549.32740999568955],
                  [0, 0, 1]])
    dist_coeffs = np.array([-0.26616998352506377,0.14393593599317869,0.00020055073869918146,0.00041759849013703147,-0.10760272994714998])

    # Undistort the points
    points_undistorted = cv2.undistortPoints(points, K, dist_coeffs)

    return points_undistorted

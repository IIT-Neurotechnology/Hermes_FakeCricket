import numpy as np


class Kalman(object):
    def __init__(self, dt, u_x, u_y, std_acc, x_std_meas, y_std_meas):
        """
        :param dt: sampling time (time for 1 cycle)
        :param u_x: acceleration in x-direction
        :param u_y: acceleration in y-direction
        :param std_acc: process noise magnitude
        :param x_std_meas: standard deviation of the measurement in x-direction
        :param y_std_meas: standard deviation of the measurement in y-direction
        """

        self.dt = dt
        self.u = np.matrix([[u_x], [u_y]])
        self.x = np.matrix([[0], [0], [0], [0]])

        # Define the State Transition Matrix A
        self.A = np.matrix([[1, 0, self.dt, 0],
                            [0, 1, 0, self.dt],
                            [0, 0, 1, 0],
                            [0, 0, 0, 1]])

        # Define the Control Input Matrix B
        self.B = np.matrix([[(self.dt**2)/2, 0],
                            [0, (self.dt**2)/2],
                            [self.dt, 0],
                            [0, self.dt]])

        # Define Measurement Mapping Matrix
        self.H = np.matrix([[1, 0, 0, 0],
                            [0, 1, 0, 0]])

        # Initial Process Noise Covariance
        self.Q = np.matrix([[(self.dt**4)/4, 0, (self.dt**3)/2, 0],
                            [0, (self.dt**4)/4, 0, (self.dt**3)/2],
                            [(self.dt**3)/2, 0, self.dt**2, 0],
                            [0, (self.dt**3)/2, 0, self.dt**2]]) * std_acc**2

        # Initial Measurement Noise Covariance
        self.R = np.matrix([[x_std_meas**2, 0],
                           [0, y_std_meas**2]])

        # Initial Covariance Matrix
        self.P = np.eye(self.A.shape[1])

    def predict(self):
            self.x = np.dot(self.A, self.x) + np.dot(self.B, self.u)
            self.P = np.dot(np.dot(self.A, self.P), self.A.T) + self.Q
            return self.x[0:2]

    def update(self, z):
            S = np.dot(self.H, np.dot(self.P, self.H.T)) + self.R
            K = np.dot(np.dot(self.P, self.H.T), np.linalg.inv(S))

            self.x = np.round(
            self.x + np.dot(K, (z - np.dot(self.H, self.x)))) 

            I = np.eye(self.H.shape[1])

            self.P = (I - (K * self.H)) * self.P
            return self.x[0:2]

import numpy as np
from sklearn.decomposition import PCA

class CricketTracker:
    def __init__(self, frame_window=10):
        self.positions = []
        self.velocities= []
        self.frame_window = frame_window
        self.angle = 0
        self.rot_matrix = np.eye(2)

    def update_cricket(self, new_position, new_velocities):
        self.positions.append(new_position.flatten())
        self.velocities.append(new_velocities.flatten())
        
        summed_vector = None
        summed_vector_v = None

        if len(self.positions) > self.frame_window and len(self.velocities) > self.frame_window:
            summed_vector = np.sum(np.diff(self.positions, axis=0), axis=0)
            print('Summed Vector:', summed_vector)
            summed_vector_v = np.sum(self.velocities, axis=0)
            print('Summed Vector vel:', summed_vector_v)
            self.velocities = []
            self.positions = []
            self.angle = self.calculate_angle(summed_vector, summed_vector_v)
            print('Theta:', self.angle)
            

        return summed_vector, summed_vector_v if (len(self.positions) == 0 and len(self.velocities) == 0) else (None, None)

    def calculate_angle(self, vector1, vector2):
        if np.all(vector1 < 10) or np.all(vector2 < 10):
            return self.angle
        cos_angle = np.dot(vector1, vector2) / (np.linalg.norm(vector1) * np.linalg.norm(vector2))
        self.angle = np.arccos(np.clip(cos_angle, -1, 1)) 
        return self.angle

    def rotation_matrix(self):  # Include 'self' in the function definition
        rad_angle = self.angle  # Use the updated angle
        self.rot_matrix = np.array([
            [np.cos(rad_angle), -np.sin(rad_angle)],
            [np.sin(rad_angle), np.cos(rad_angle)]
            ])
        return self.rot_matrix
    
    def calibrate(self, commanded_vectors, actual_vectors):
        # Concatenate the vectors into one matrix
        combined_vectors = np.hstack((commanded_vectors, actual_vectors))

        # Perform PCA on the combined matrix
        pca = PCA(n_components=2)
        pca.fit(combined_vectors)

        # The first principal component is the direction of maximum variance.
        # The rotation angle is the angle this vector makes with the x-axis.
        rotation_angle = np.arctan2(pca.components_[0, 1], pca.components_[0, 0])

        # Create and store a rotation matrix based on the computed angle.
        self.rotation_matrix = np.array([
            [np.cos(rotation_angle), -np.sin(rotation_angle)],
            [np.sin(rotation_angle), np.cos(rotation_angle)]
        ])

    def adjust_command(self, command_vector):
        # Multiply the command vector by the rotation matrix to adjust it.
        adjusted_command = np.dot(self.rotation_matrix, command_vector)
        return adjusted_command




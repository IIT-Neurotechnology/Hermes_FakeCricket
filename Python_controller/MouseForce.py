import numpy as np

def rat_repulsion_force(point, rat_center, k_rat=9000, k_max_dist=500):
    """Calculates the repulsion force at a given point due to the rat's center.
    
    Args:
        point (tuple): Coordinates of the point (x, y).
        rat_center (tuple): Coordinates of the rat's center (x, y).
        k_rat (float): Repulsion strength for the rat's center (default: 10000).
        k_max_dist (float): Maximum distance to apply repulsion (default: 700).
        
    Returns:
        numpy.ndarray: Repulsion force as a tuple (x, y).
    """
    x, y = point
    rat_x, rat_y = rat_center
    dist_x = x - rat_x
    dist_y = y - rat_y
    dist = np.sqrt(dist_x**2 + dist_y**2)

    force = k_rat * (1 - dist / k_max_dist)
    force = np.clip(force, 0, None)  # Only apply repulsion within k_max_dist
    force = force / np.maximum(1, dist)  # Inverse distance weighting

    direction = np.array([dist_x, dist_y]) / np.maximum(1, dist)
    repulsion = force * direction
    return repulsion

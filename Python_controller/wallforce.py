import numpy as np
import matplotlib.pyplot as plt


def calculate_force_field(width, height, force_const):
    # Create 2D arrays for the x and y coordinates
    x, y = np.meshgrid(np.arange(width), np.arange(height))

    # Calculate the distances from the four walls
    dist_left = x
    dist_right = (width) - x
    dist_top = y 
    dist_bottom = (height) - y

    # Get the minimum distance either from left or right, and top or bottom
    x_closest_dist = np.maximum(dist_left, dist_right)
    y_closest_dist = np.maximum(dist_bottom, dist_top)

    # Prevent division by zero by adding a very small value to the distances
    x_closest_dist = np.where(x_closest_dist == 0, 0.1, x_closest_dist)
    y_closest_dist = np.where(y_closest_dist == 0, 0.1, y_closest_dist)

    # Forces are proportional to the inverse of the distance
    x_force = force_const * np.power(x_closest_dist,3)
    y_force = force_const * np.power(y_closest_dist,3)

    # Forces should be oriented from the walls towards the center
    x_force = np.where(x > width / 2, -x_force, x_force)
    y_force = np.where(y > height / 2, -y_force, y_force)

    return x_force, y_force


def plot_force_field(x_force, y_force, debugmode):
    # Plot x_force
    plt.figure(figsize=(6, 6))
    plt.imshow(x_force, origin='lower', cmap='RdBu')
    plt.colorbar(label='x_force')
    plt.title('x_force')
    plt.show()

# Plot y_force
    plt.figure(figsize=(6, 6))
    plt.imshow(y_force, origin='lower', cmap='RdBu')
    plt.colorbar(label='y_force')
    plt.title('y_force')
    plt.show()
    
    if debugmode==1:
        plt.figure(figsize=(7,7))
        #We scale the arrows to avoid small arrows in high force regions
        M = np.hypot(x_force, y_force)
    
        plt.quiver(x_force, y_force, M, angles='xy', scale_units='xy', scale=10)
        plt.title('Force field')
        plt.show()

import numpy as np
import math

def Kinematic(force):
    d = 0.1
 # alpha = math.pi/6
    AI = np.matrix([ [-0.5 ,-math.sqrt(3)/2, d],
                     [-0.5, math.sqrt(3)/2, d],
                     [1, 0, d]])
    Mvel = np.ones(3)
    Mvel = np.matmul(AI, force)
    # Convert to integers
    Mvel = Mvel.astype(int)
    # Clip the values of the transformed force to the range [-255, 255]
    Mvel = np.clip(Mvel, -255, 255)
    
    return Mvel

def Kinematic_test(force):
    d = 0.1
 # alpha = math.pi/6
    AI = np.matrix([[1, 0, 0],
                    [0, 1, 0],
                    [0, 0, 1]])
    Mvel = np.ones(3)
    Mvel = np.matmul(AI, force)
    
    # Clip the values of the transformed force to the range [-255, 255]
    Mvel = np.clip(Mvel, -255, 255)
    
    return Mvel
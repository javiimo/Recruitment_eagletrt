
import numpy as np
import math
#------------------------------------------
#DECLARING DATA VARIABLES:

# Intrinsic Parameters
fx = 241  # Focal length in the x direction
fy = 238  # Focal length in the y direction
cx = 636  # Principal point's x coordinate
cy = 548  # Principal point's y coordinate

# Object's Distance (in m) from the Camera Frame
d = 2.7

# Pixel coordinate's of the object's center
u=795
v=467

# Euler Angles in radians
roll = math.radians(100)
pitch = math.radians(0)
yaw = math.radians(90)

#Define the rotation matrice and get the inverse
R = np.array([[math.cos(yaw)*math.cos(pitch), -math.sin(yaw)*math.cos(roll) + math.cos(yaw)*math.sin(pitch)*math.sin(roll), math.sin(yaw)*math.sin(roll) + math.cos(yaw)*math.sin(pitch)*math.cos(roll)],
              [math.sin(yaw)*math.cos(pitch), math.cos(yaw)*math.cos(roll) + math.sin(yaw)*math.sin(pitch)*math.sin(roll), -math.cos(yaw)*math.sin(roll) + math.sin(yaw)*math.sin(pitch)*math.cos(roll)],
              [-math.sin(pitch), math.cos(pitch)*math.sin(roll), math.cos(pitch)*math.cos(roll)]])
R=R.T #It is orthonormal, the traspose is the inverse as well

#Traslation vector in meters:
tras=np.array([0.5,0.16,1.140])
#---------------------------------------
# CALCULATIONS:

# Normalized 2D Homogeneous Coordinates
Norm_x = (u - cx)/fx
Norm_y = (u - cy)/fy

# Construct 2D Homogeneous Coordinate
Homo_2D_Cam = np.array([Norm_x, Norm_y, 1])

# Construct the Camera Matrix
K = np.array([[fx, 0, -cx],
              [0, fy, -cy],
              [0, 0, 1]])

# Perform 2D to 3D Transformation in camera frame
Homo_3D_Cam = np.linalg.inv(K).dot(Homo_2D_Cam)

# Apply Distance Scaling
Euclid_3D_Cam = Homo_3D_Cam * d

#Apply inverse rotations:
Inv_Rot = np.dot(R, Euclid_3D_Cam)

#Apply the traslation
Euclid_3D_Veh=Inv_Rot+tras

#Print the result
x, y, z = Euclid_3D_Veh
print(f"3D Point in Vehicle Frame (meters): x={x}, y={y}, z={z}")

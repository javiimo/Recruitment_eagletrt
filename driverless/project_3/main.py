#%
import laspy
import numpy as np
import open3d as o3d

# Load the 3xN point cloud data
in_las=laspy.read('broken.las')

# Extract the data (double type) as a Nx3 matrix
xyz = np.vstack((in_las.x, in_las.y, in_las.z)).T

# Get the point count
print('Point count: ', in_las.header.point_count)

#Get the box dimensions
s='Box Dimensions: '
for i in range(3):
    s=s+'['+str(np.min(xyz[:,i]))+','+str(np.max(xyz[:,i]))+']'
    if i!=2:
        s=s+'x'
print(s)

# Try the 2 different reshape function orders to get the 2 possible 3xN data.
original=[]
N=in_las.header.point_count
for M in ['C','F']: #C-like or Fortran-like
    original.append(np.reshape(xyz,(3,N) , order=M).T)


# Visualize the 2 possibilities
for i in range(2):
    point_cloud = o3d.geometry.PointCloud()
    point_cloud.points = o3d.utility.Vector3dVector(original[i])

    o3d.visualization.draw_geometries([point_cloud])

#As we can see, the F-like gives the same image scrambled, so it is the C-like the one that descramble the data


# We save the fixed data to a new .las file which contains the same metadata
out_las = laspy.LasData(in_las.header)

out_las.x = original[0][:, 0]
out_las.y = original[0][:, 1]
out_las.z = original[0][:, 2]


# Save the fixed .las file
out_las.write("fixed.las")


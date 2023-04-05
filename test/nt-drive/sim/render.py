import cv2
import numpy as np


# TODO: turn into function
# Generate a black image
img = np.zeros((480, 640, 3), dtype=np.uint8)
img[:, :] = (255,0,20)
k=1
# Define the rectangle corners in 3D space
pixel_pos=[
    [[-4*k,3*k,0],[-4*k,4*k,0],[4*k,4*k,0],[4*k,3*k,0]],
    [[-4*k,-3*k,0],[-4*k,-4*k,0],[4*k,-4*k,0],[4*k,-3*k,0]],
    [[-4*k,-3*k,0],[-3*k,-3*k,0],[-3*k,3*k,0],[-4*k,3*k,0]],
    [[4*k,-3*k,0],[3*k,-3*k,0],[3*k,3*k,0],[4*k,3*k,0]],
    [[-3*k,2*k,0],[-3*k,3*k,0],[3*k,3*k,0],[3*k,2*k,0]],
    [[-3*k,-2*k,0],[-3*k,-3*k,0],[3*k,-3*k,0],[3*k,-2*k,0]],
    [[-3*k,-2*k,0],[-2*k,-2*k,0],[-2*k,2*k,0],[-3*k,2*k,0]],
    [[3*k,-2*k,0],[2*k,-2*k,0],[2*k,2*k,0],[3*k,2*k,0]],

    [[-2*k,-2*k,0],[-2*k,-1*k,0],[-1*k,-1*k,0],[-1*k,-2*k,0]],
    [[-1*k,-2*k,0],[-1*k,-1*k,0],[0*k,-1*k,0],[0*k,-2*k,0]],
    [[0*k,-2*k,0],[0*k,-1*k,0],[1*k,-1*k,0],[1*k,-2*k,0]],
    [[1*k,-2*k,0],[1*k,-1*k,0],[2*k,-1*k,0],[2*k,-2*k,0]],

    [[-2*k,-1*k,0],[-2*k,0*k,0],[-1*k,0*k,0],[-1*k,-1*k,0]],
    [[-1*k,-1*k,0],[-1*k,0*k,0],[0*k,0*k,0],[0*k,-1*k,0]],
    [[0*k,-1*k,0],[0*k,0*k,0],[1*k,0*k,0],[1*k,-1*k,0]],
    [[1*k,-1*k,0],[1*k,0*k,0],[2*k,0*k,0],[2*k,-1*k,0]],

    [[-2*k,0*k,0],[-2*k,1*k,0],[-1*k,1*k,0],[-1*k,0*k,0]],
    [[-1*k,0*k,0],[-1*k,1*k,0],[0*k,1*k,0],[0*k,0*k,0]],
    [[0*k,0*k,0],[0*k,1*k,0],[1*k,1*k,0],[1*k,0*k,0]],
    [[1*k,0*k,0],[1*k,1*k,0],[2*k,1*k,0],[2*k,0*k,0]],

    [[-2*k,1*k,0],[-2*k,2*k,0],[-1*k,2*k,0],[-1*k,1*k,0]],
    [[-1*k,1*k,0],[-1*k,2*k,0],[0*k,2*k,0],[0*k,1*k,0]],
    [[0*k,1*k,0],[0*k,2*k,0],[1*k,2*k,0],[1*k,1*k,0]],
    [[1*k,1*k,0],[1*k,2*k,0],[2*k,2*k,0],[2*k,1*k,0]],
]
pixel_val=[
    1,1,1,1, # outer deadzone
    0,0,0,0, # inner deadzone

    0,0,1,0,
    1,1,1,0,
    1,0,1,0,
    0,1,0,1
]

# Project the rectangle corners onto the image plane
focal_length = 500
principal_point = (320, 240)
camera_matrix = np.array([
    [focal_length, 0, principal_point[0]],
    [0, focal_length, principal_point[1]],
    [0, 0, 1]
], dtype=np.float32)

rotation_vector = np.array([40, 0, 0], dtype=np.float32) # pitch,yaw,roll
translation_vector = np.array([4, 3, 30], dtype=np.float32) # lr,ud,dist
for pts,color in zip(pixel_pos,pixel_val):
    projected_points, _ = cv2.projectPoints(np.array([np.array(i) for i in pts], dtype=np.float32), np.deg2rad(rotation_vector), translation_vector, camera_matrix, None)

    # Draw the rectangle on the image
    points = projected_points.astype(np.int32).reshape((-1, 2))
    cv2.fillConvexPoly(img, points, (255, 255, 255) if color else (0,0,0))

# Save the image to a PNG file
if __name__=="__main__":
    cv2.imshow('Output Image', img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    cv2.imwrite('april.png', img)

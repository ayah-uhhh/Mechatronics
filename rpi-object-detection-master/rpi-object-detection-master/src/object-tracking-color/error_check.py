# center 176, 126 of green
# import the (cx,cy) being ran by cv_object_tracking_color.py 
# check
import numpy as np 
from cv_object_tracking_color import *

target_x = 176
target_y = 126


while True:
    cam_sees = get_target()
    if cam_sees is not None:
        cx, cy = cam_sees
        print(f"Central position: ({cx}, {cy})")
        
        # Calculate the distance
        distance = np.sqrt((cx - target_x) ** 2 + (cy - target_y) ** 2)
        if distance != 0:
            if cx < target_x:
                print("move right") #bot motion
            if cx > target_x:
                print("move left")
            if cy > target_y:
                print("move down") # ramp motion
            if cy < target_y:
                print("move up") 
    else:
        print("Central position not found.")


import cv2
import numpy as np
import time
import csv
import serial

CAMERA_DEVICE_ID = 0
IMAGE_WIDTH = 320
IMAGE_HEIGHT = 240
counter = 1

cap = cv2.VideoCapture(0) # video capture source camera (Here webcam of laptop) 
ret,frame = cap.read() # return a single frame in variable `frame`
cv2.imwrite('shapes.jpg',frame)

# Load the image
image = cv2.imread('shapes.jpg')

# Convert image to HSV color space
hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
#green_pixel_rgb = (201,226,153)
hsv_value = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
lower = hsv_value - (50,10,20)
upper = hsv_value + (50,10,20)
#print(hsv_value)
# Define lower and upper bounds for green color in HSV
lower_green = (30,100,100)
upper_green = (80,215,215)
print(lower_green)
#print(upper_green)

# Create a binary mask for the green color
green_mask = cv2.inRange(hsv_image, lower_green, upper_green)

result_image = cv2.bitwise_and(image,image, mask= green_mask)

#save result image
cv2.imwrite("mask_file.jpg",result_image)

img = cv2.imread("mask_file.jpg")

gray_image = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
ret, thresh = cv2.threshold(gray_image, 30, 255, 0)
cv2.imshow("Gray",gray_image)
# Find contours in the binary mask
contours,hierarchy = cv2.findContours(thresh, cv2.RETR_TREE,cv2.CHAIN_APPROX_NONE)
print("Number of contours detected:",len(contours))

# Create a new blank image

for cnt in contours:
  img = cv2.drawContours(img, [cnt], -1, (0,255,255), 3)

  # compute the center of mass of the triangle
  M = cv2.moments(cnt)
  if M['m00'] != 0.0:
     x = int(M['m10']/M['m00'])
     y = int(M['m01']/M['m00'])
     cv2.circle(img,(x,y),5,255,-1)
     print(x)
     print(y)

cv2.imshow("Shapes", img)
cv2.waitKey(0)
cv2.destroyAllWindows()


def error_check(x, y):
    cx=x;
    cy=y;
    target_x = 300
    target_y = 198
    # Calculate the distance
    if color_state == 1 or 2:
        # distance = np.sqrt((cx - target_x) ** 2 + (cy - target_y) ** 2)
        x_dist = abs(cx - target_x)
        y_dist = abs(cy - target_y)
        if x_dist > 10:
            if cx < target_x:
                print("move right") #bot motion
                command = "r"
                #motor_cmd_m(command)
            if cx > target_x:
                print("move left")
                command = "l"
                #motor_cmd_m(command)
        else:
            print("You got X!")
        if y_dist > 10:
            if cy > target_y:
                print("move down") # ramp motion
                command = "d"
                #motor_cmd_s(command)
            if cy < target_y:
                print("move up")
                 #motor_cmd_s(command)
        else:
            print("You got Y!")

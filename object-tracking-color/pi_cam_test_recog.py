import cv2
import numpy as np
import time
import csv
import serial

CAMERA_DEVICE_ID = 0
IMAGE_WIDTH = 320
IMAGE_HEIGHT = 240
counter = 1

color_state = 1 #target state = 1 means target the triangle; 2 = circle; 3 = pins
target_aquired = 0

def motor_cmd_s(arg):
    ser_s.reset_input_buffer()
    ser_s.write(bytes(arg, 'utf-8'))
    line = ser_s.readline().decode('utf-8').rstrip()
    time.sleep(1)

def motor_cmd_m(arg):
    ser_m.reset_input_buffer()
    ser_m.writer(bytes(arg, 'utf-8'))
    line = ser_m.readline().decode('utf-8').rstrip()
    #print(line)
    time.sleep(1)
    
def send_command_and_ack(ser, command, ack):
    ser.reset_input_buffer()
    ser.write(bytes(command, 'utf-8'))
    while ser.readline().decode('utf-8').rstrip() !=ack: #this will change based on what "ack" you place in the function
        pass
    time.sleep(1)

#def target_aim(color_state):
if color_state == 1: # this means we are targetting the TRIANGLE
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
    #print(lower_green)
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
        target_aquired = 1 #have it add one so we know to move on to the next target????

    cv2.imshow("Shapes", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def error_check(x, y):
    cx=x;
    cy=y;
    get_x = 0
    get_y = 0
    if color_state == 1:
        target_x = 313
        target_y = 182
    if color_state == 2: #will be updated
        target_x = -1
        target_y = -1
    if color_state == 3: #willl also be updated
        target_x = -1
        target_y = -1
    # Calculate the distance
    if color_state == 1 or 2:
        # distance = np.sqrt((cx - target_x) ** 2 + (cy - target_y) ** 2)
        x_dist = abs(cx - target_x)
        y_dist = abs(cy - target_y)
        if x_dist > 5:
            if cx < target_x:
                print("move right") #bot motion
                command = "r"
                time.sleep(2)
                motor_cmd_m(command)
            if cx > target_x:
                print("move left")
                command = "l"
                time.sleep(2)
                motor_cmd_m(command)
        else:
            print("You got X!")
            get_x = 1
        if y_dist > 5:
            if cy > target_y:
                print("move down") # ramp motion
                command = "d"
                time.sleep(1)
                motor_cmd_s(command)
            if cy < target_y:
                print("move up")
                time.sleep(1)
                motor_cmd_s(command)
        else:
            print("You got Y!")
            get_y = 1
    return(get_x, get_y)

if __name__ == "__main__":
    ser_s = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser_m = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
    ser_s.reset_input_buffer()
    ser_m.reset_input_buffer()
    ardcmd = 1
    while True:
        if ardcmd == 1:
                # Send 's' command to Arduino and wait for 'AckS'
            #print("start")
            time.sleep(3)
            send_command_and_ack(ser_m, 's', 'Acks')
            
            # Send 'c' three times and wait for 'AckC' after each command
            for _ in range(3):
                print("Corner")
                motor_cmd_m('c')
                send_command_and_ack(ser_m, 'c', 'Ackc')

            # Send 'uuuuu' to Arduino and wait for 'AckUUUUU'
            for _ in range(5):
                print("Move up")
                motor_cmd_s('u')
                send_command_and_ack(ser_s, 'u', 'Acku')

            # Start error check code
            error_check(x, y)
            
            if error_check.get_x == 1 and error_check.get_y == 1:
                print("Yeet")
                motor_cmd_s('y')
                send_command_and_ack(ser_s, 'y', 'Acky')
            """
               print("ready")
                time.sleep(3)
                print("Move Left") #moves wheels to the left, with respect of the camera
                command = "l"
                motor_cmd_s(command)
                time.sleep(1)
                print("Move Right") #moves wheels to the right, with respect of the camera
                command = "r"
                motor_cmd_s(command)
                time.sleep(1)
                print("Move Up") #moves the angle of the servo up 5 degrees
                command = "u"
                motor_cmd_s(command)
                time.sleep(1)
                print("Move Down") #moves the angle of the servo down 5 degrees
                command = "d"
                motor_cmd_s(command)
                time.sleep(1)
                print("Yeet") #turns on shooting motors
                command = "y"
                motor_cmd_s(command)
                time.sleep(1)
                print("Zero") # the angle of the servo goes to zero
                command = "z"
                motor_cmd_s(command)
                time.sleep(1)
                print("Kill") #turns off shooting motors
                command = "k"
                motor_cmd_s(command)
                time.sleep(1)
                print("Start") #takes in button push, begin to drive to first target
                command = "s"
                motor_cmd_s(command)
                ardcmd == 0
                """

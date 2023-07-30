#!/usr/bin/python3

# ------------------------------------------------------------------------------
# This is a blob detection program which intend to find the biggest blob
# in a given picture taken by a camera and return its central position.
# 
# Key Steps:
# 1. Image Filtering
# 2. Image Segmentation
# 3. Detect Blobs
# 4. Filter Blobs using a criteria
# 5. Track Blobs
# ------------------------------------------------------------------------------
# automaticdai
# YF Robotics Labrotary
# Instagram: yfrobotics
# Twitter: @yfrobotics
# Website: https://www.yfrl.org
# ------------------------------------------------------------------------------
# center 176, 126 of green
import cv2
import numpy as np
import time
import csv
import serial

color_state = 0
learning = 0
CAMERA_DEVICE_ID = 0
IMAGE_WIDTH = 320
IMAGE_HEIGHT = 240
fps = 0

hsv_min = np.array((0, 0, 0))
hsv_max = np.array((255, 255, 255))

if color_state == 0: #GET THE PINKS 161 
    colors = []
    color_file = 'pink_hardcode.csv'
    target_x = 176
    target_y = 126

if color_state == 1: #GET THE GREENS
    colors = []
    color_file = 'green_hardcode.csv'
    target_x = 176
    target_y = 126

if color_state == 2: #GET THE BLUES 287, 161
    colors = []
    color_file = 'blue_hardcode.csv'
    target_x = 194
    target_y = 163
cx = None
cy = None


def motor_cmd_s(arg):
    ser_s.reset_input_buffer()
    ser_s.write(bytes(arg, 'utf-8'))
    line = ser_s.readline().decode('utf-8').rstrip()
    time.sleep(1)
def motor_cmd_m(arg):
    ser_m.reset_input_buffer()
    ser_m.writer(bytes(arg, 'utf-8'))
    line = ser_m.readline().decode('utf-8').rstrip()
    print(line)
    time.sleep(1)

def error_check(cx, cy):
    # Calculate the distance
    if color_state == 1 or 2:
        # distance = np.sqrt((cx - target_x) ** 2 + (cy - target_y) ** 2)
        x_dist = abs(cx - target_x)
        y_dist = abs(cy - target_y)
        if x_dist > 10:
            if cx < target_x:
                print("move right") #bot motion
                command = "r"
                motor_cmd_m(command)
            if cx > target_x:
                print("move left")
                command = "l"
                motor_cmd_m(command)
        else:
            print("You got X!")
            command = "p"
            motor_cmd_s(command)
        if y_dist > 10:
            if cy > target_y:
                print("move down") # ramp motion
                command = "d"
                motor_cmd_s(command)
            if cy < target_y:
                print("move up")
                command = "u"
                motor_cmd_s(command)
        else:
            print("You got Y!")

def isset(v):
    try:
        type (eval(v))
    except:
        return 0
    else:
        return 1


def on_mouse_click(event, x, y, flags, frame):
    global colors

    if event == cv2.EVENT_LBUTTONUP:
        color_bgr = frame[y, x]
        color_rgb = tuple(reversed(color_bgr))
        #frame[y,x].tolist()

        # print(color_rgb)

        color_hsv = rgb2hsv(color_rgb[0], color_rgb[1], color_rgb[2])
        # print(color_hsv)
        
        colors.append(color_hsv)
        item_length = len(colors)
        with open(color_file, 'w') as f:
            file_writer = csv.writer(f)
            for color in colors:
                file_writer.writerow(color)
    
        print(colors)
        


# R, G, B values are [0, 255]. 
# Normally H value is [0, 359]. S, V values are [0, 1].
# However in opencv, H is [0,179], S, V values are [0, 255].
# Reference: https://docs.opencv.org/3.4/de/d25/imgproc_color_conversions.html
def hsv2rgb(h, s, v):
    h = float(h) * 2
    s = float(s) / 255
    v = float(v) / 255
    h60 = h / 60.0
    h60f = math.floor(h60)
    hi = int(h60f) % 6
    f = h60 - h60f
    p = v * (1 - s)
    q = v * (1 - f * s)
    t = v * (1 - (1 - f) * s)
    r, g, b = 0, 0, 0
    if hi == 0: r, g, b = v, t, p
    elif hi == 1: r, g, b = q, v, p
    elif hi == 2: r, g, b = p, v, t
    elif hi == 3: r, g, b = p, q, v
    elif hi == 4: r, g, b = t, p, v
    elif hi == 5: r, g, b = v, p, q
    r, g, b = int(r * 255), int(g * 255), int(b * 255)
    return (r, g, b)


def rgb2hsv(r, g, b):
    r, g, b = r/255.0, g/255.0, b/255.0
    mx = max(r, g, b)
    mn = min(r, g, b)
    df = mx-mn
    if mx == mn:
        h = 0
    elif mx == r:
        h = (60 * ((g-b)/df) + 360) % 360
    elif mx == g:
        h = (60 * ((b-r)/df) + 120) % 360
    elif mx == b:
        h = (60 * ((r-g)/df) + 240) % 360
    if mx == 0:
        s = 0
    else:
        s = df/mx
    v = mx

    h = int(h / 2)
    s = int(s * 255)
    v = int(v * 255)

    return (h, s, v)


def visualize_fps(image, fps: int):
    if len(np.shape(image)) < 3:
        text_color = (255, 255, 255)  # white
    else:
        text_color = (0, 255, 0)  # green
    row_size = 20  # pixels
    left_margin = 24  # pixels

    font_size = 1
    font_thickness = 1

    # Draw the FPS counter
    fps_text = 'FPS = {:.1f}'.format(fps)
    text_location = (left_margin, row_size)
    cv2.putText(image, fps_text, text_location, cv2.FONT_HERSHEY_PLAIN,
                font_size, text_color, font_thickness)

    return image
import pandas as pd
color = None
def check_colors(pixel):
    color_ranges = {
        'Red': [(150, 0, 0), (190, 255, 255)],
        'Green': [(40, 0, 0), (95, 255, 255)],
        'Blue': [(100, 0, 0), (130, 255, 255)]
    }
    for color, (lower, upper) in color_ranges.items():
        if np.all(pixel >= lower) and np.all(pixel <= upper):
            return color
        else:
            print("get out of the way")
    return None

if __name__ == "__main__":
    ser_s = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser_m = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
    ser_s.reset_input_buffer()
    ser_m.reset_input_buffer()
    ardcmd = 1
    
    try:
        # create video capture
        if ardcmd == 0:
            cap = cv2.VideoCapture(CAMERA_DEVICE_ID)
    
            # set resolution to 320x240 to reduce latency 
            cap.set(3, IMAGE_WIDTH)
            cap.set(4, IMAGE_HEIGHT)
            if learning == 0:
                with open(color_file, 'r') as file:
                    csvreader = csv.reader(file)
                    for row in csvreader:
                        row = [int(value) for value in row]
    
                        colors.append(tuple(row))
                # print(colors)
        while True:
            # ----------------------------------------------------------------------
            # record start time
            # start_time = time.time()--
            # Read the frames frome a camera
            # _, frame = cap.read() --
            # frame = cv2.blur(frame,(3,3))--

            # Or get it from a JPEG
            # frame = cv2.imread('frame0010.jpg', 1)

            # Convert the image to hsv space and find range of colors
            # hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV) --
            # cv2.namedWindow('frame') --
            # cv2.setMouseCallback('frame', on_mouse_click, frame) --
            if ardcmd == 1:
                print("ready")
                time.sleep(3)
                print("Move Left")
                command = "l"
                motor_cmd_s(command)
                time.sleep(1)
                print("Move Right")
                command = "r"
                motor_cmd_s(command)
                time.sleep(1)
                print("Move Up")
                command = "u"
                motor_cmd_s(command)
                time.sleep(1)
                print("Move Down")
                command = "d"
                motor_cmd_s(command)
                time.sleep(1)
                print("Fire")
                command = "p"
                motor_cmd_s(command)
                time.sleep(1)
                ardcmd == 0

        
            # Uncomment this for RED tag
            # thresh = cv2.inRange(hsv,np.array((120, 80, 80)), np.array((180, 255, 255)))

            # find the color using a color threshhold
            if colors:
                # find max & min h, s, v
                minh = min(c[0] for c in colors)
                mins = min(c[1] for c in colors)
                minv = min(c[2] for c in colors)
                maxh = max(c[0] for c in colors)
                maxs = max(c[1] for c in colors)
                maxv = max(c[2] for c in colors)

                # print("New HSV threshold: ", (minh, mins, minv), (maxh, maxs, maxv))
                hsv_min = np.array((minh, mins, minv))
                hsv_max = np.array((maxh, maxs, maxv))
                # print("hsvmin=",hsv_min)
            

            thresh = cv2.inRange(hsv, hsv_min, hsv_max)
            thresh2 = thresh.copy()

            # find contours in the threshold image
            (major_ver, minor_ver, subminor_ver) = (cv2.__version__).split('.')
            #print(major_ver, minor_ver, subminor_ver)

            # findContours() has different form for opencv2 and opencv3
            if major_ver == "2" or major_ver == "3":
                _, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
            else:
                contours, hierarchy = cv2.findContours(thresh, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

            # finding contour with maximum area and store it as best_cnt
            max_area = 0
            for cnt in contours:
                area = cv2.contourArea(cnt)
                if area > max_area:
                    max_area = area
                    best_cnt = cnt
                    #print(hsv)
                    # print("color = ",color)
                

            # finding centroids of best_cnt and draw a circle there
            if isset('best_cnt'):
                M = cv2.moments(best_cnt)
                cx,cy = int(M['m10']/M['m00']), int(M['m01']/M['m00'])
                cv2.circle(frame,(cx,cy),5,255,-1)
                # print("Central pos: (%d, %d)" % (cx,cy))
                
                hsv_pixel = hsv[cy,cx]
                color = check_colors(hsv_pixel)
                if color is not None:
                    trigger = {'Red': 1, 'Green': 2, 'Blue': 3}[color]
                    print("Detected color:", color)
                    print((cx,cy))
            
            else:
                trigger = 0
                # print("[Warning]Tag lost...")
                
            # Show the original and processed image
            #res = cv2.bitwise_and(frame, frame, mask=thresh2)
            cv2.imshow('frame', visualize_fps(frame, fps))
            cv2.imshow('thresh', visualize_fps(thresh2, fps))
            # ----------------------------------------------------------------------
            # record end time
            end_time = time.time()
            # calculate FPS
            seconds = end_time - start_time
            fps = 1.0 / seconds
            
            print(f"Central position: ({cx}, {cy})")
            #print("Estimated fps:{0:0.1f}".format(fps));
            # if key pressed is 'Esc' then exit the loop
            error_check(cx,cy)
            if cv2.waitKey(33) == 27:
                break
            
    except Exception as e:
        print(e)




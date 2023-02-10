import cv2
import Vision
from flask import Flask, render_template, Response, redirect, request

__COLOR_PICK_RANGE__ = 10

__current_frame__ = None
__test_camera__ = None

# just a simple demo, bring this into production use later

def index():
    """Video streaming home page.""" 
    return render_template('picker.html')

# Test functions, replace later
def read_camera():
    global __current_frame__
    global __test_camera__
    __test_camera__ = cv2.VideoCapture("/dev/video0")
    __CAM_WIDTH__ = __test_camera__.get(cv2.CAP_PROP_FRAME_WIDTH)
    __CAM_HEIGHT__ = __test_camera__.get(cv2.CAP_PROP_FRAME_HEIGHT)

    while True:
        ret, frame = __test_camera__.read()

        if not ret:
            break

        __current_frame__ = frame.copy()

        #draw color picker rectangle
        x = int(__CAM_WIDTH__ / 2)
        y = int(__CAM_HEIGHT__ / 2)
        cv2.rectangle(frame, (x - __COLOR_PICK_RANGE__, y - __COLOR_PICK_RANGE__),
                             (x + __COLOR_PICK_RANGE__, y + __COLOR_PICK_RANGE__),
                             (0,255,0), 2)
        #crosshairs
        cv2.rectangle(frame, (x - 1, y - __COLOR_PICK_RANGE__ * 2),
                             (x + 1, y + __COLOR_PICK_RANGE__ * 2),
                             (0,255,0), 2)
        cv2.rectangle(frame, (x - __COLOR_PICK_RANGE__ * 2, y - 1),
                             (x + __COLOR_PICK_RANGE__ * 2, y + 1),
                             (0,255,0), 2)

        # This step encodes the data into a jpeg image
        ret, buffer = cv2.imencode('.jpg', frame) 

        # We have to return bytes to the user
        frame = buffer.tobytes() 

        # Return the image to the browser
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')  # concat frame one by one and show result

def camera_feed(): 
    return Response(read_camera(), mimetype='multipart/x-mixed-replace; boundary=frame')

def pick():
    __test_camera__.release()
    col = Vision.averageColor(__current_frame__, __COLOR_PICK_RANGE__)
    print("color: ", col)
    #write to file
    file = open("picked_color", "w")
    file.write(str(int(col[0])) + " " + str(int(col[1])) + " " + str(int(col[2])) + " \n")
    file.close()
    return redirect("/")

if __name__ == "__main__":
    # will only run if this is being run as main
    # no code here will run, use this for testing   

    app = Flask(__name__)
    app.route("/")(index)
    app.route("/camera_feed")(camera_feed)
    app.route("/pick")(pick)
    app.run(threaded=True, host="0.0.0.0")
    pass
else:
    pass

#Author-
#Description-

import adsk.core, adsk.fusion, adsk.cam, traceback
import threading, random, json, serial

app = None
ui = adsk.core.UserInterface.cast(None)
view = None
handlers = []
stopFlag = None
myCustomEvent = 'MyCustomEventId'
customEvent = None


# The event handler that responds to the custom event being fired.
class ThreadEventHandler(adsk.core.CustomEventHandler):
    def __init__(self):
        super().__init__()
    def notify(self, args):
        try:
            # Make sure a command isn't running before changes are made.
            if ui.activeCommand != 'SelectCommand':
                ui.commandDefinitions.itemById('SelectCommand').execute()
                            
            # Get the value from the JSON data passed through the event.
            eventArgs = json.loads(args.additionalInfo)
            translatedMatrix = eventArgs['Value']

            matrix = adsk.core.Matrix3D.create()
            matrix.setWithArray(translatedMatrix)

            camera = view.camera
            camera_eye = camera.eye
            target = camera.target

            distance = camera_eye.distanceTo(target)

            camera_eye.transformBy(translatedMatrix)


            camera.eye = camera_eye
            camera.target = target

            camera.isSmoothTransition = False

            view.camera = camera

            adsk.doEvents()

            view.refresh()
        except:
            if ui:
                ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))


# The class for the new thread.
class MyThread(threading.Thread):
    def __init__(self, event):
        threading.Thread.__init__(self)
        self.stopped = event

    def run(self):
        # Every five seconds fire a custom event, passing a random number.

        serialInst = serial.Serial()
        serialInst.baudrate = 9600
        serialInst.port = "COM4"
        serialInst.open()

        l0 = 0
        l1 = 0
        l2 = 0
        l3 = 0

        while not self.stopped.wait(5):
            mpuQuat = serialInst.readline().decode('utf-8').strip().split('\t')

            q0 = float(mpuQuat[0])
            q1 = float(mpuQuat[1])
            q2 = float(mpuQuat[2])
            q3 = float(mpuQuat[3])

            a11 = 2 * (q0 * q0 + q1 * q1) - 1
            a12 = 2 * (q1 * q2 - q0 * q3)
            a13 = 2 * (q1 * q3 + q0 * q2)
            a14 = 0
            a21 = 2 * (q1 * q2 + q0 * q3)
            a22 = 2 * (q0 * q0 + q2 * q2) - 1
            a23 = 2 * (q2 * q3 - q0 * q1)
            a24 = 0
            a31 = 2 * (q1 * q3 - q0 * q2)
            a32 = 2 * (q2 * q3 + q0 * q1)
            a33 = 2 * (q0 * q0 + q3 * q3) - 1
            a34 = 0
            a41 = 0
            a42 = 0
            a43 = 0
            a44 = 1

            l0 = q0
            l1 = q1
            l2 = q2
            l3 = q3

            translatedMatrix = [a11,a12,a13,a14,a21,a22,a23,a24,a31,a32,a33,a34,a41,a42,a43,a44]

            args = {'Value': translatedMatrix}

            app.fireCustomEvent(myCustomEvent, json.dumps(args)) 
        
        
def run(context):
    global ui
    global app
    global view
    global design
    try:
        app = adsk.core.Application.get()
        ui  = app.userInterface
        view = app.activeViewport
        

        # Register the custom event and connect the handler.
        global customEvent
        customEvent = app.registerCustomEvent(myCustomEvent)
        onThreadEvent = ThreadEventHandler()
        customEvent.add(onThreadEvent)
        handlers.append(onThreadEvent)

        # Create a new thread for the other processing.        
        global stopFlag        
        stopFlag = threading.Event()
        myThread = MyThread(stopFlag)
        myThread.start()
    except:
        if ui:
            ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))


def stop(context):
    try:
        if handlers.count:
            customEvent.remove(handlers[0])
        stopFlag.set() 
        app.unregisterCustomEvent(myCustomEvent)
        ui.messageBox('Stop addin')
    except:
        if ui:
            ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))
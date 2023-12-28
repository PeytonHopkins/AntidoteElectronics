import adsk.core, adsk.fusion, traceback, math, serial, serial.tools.list_ports, time
import threading

app = None
ui = adsk.core.UserInterface.cast(None)
handlers = []
stopFlag = None
myCustomEvent = 'MyCustomEventId'
customEvent = None

def move_camera():
    try:    
        rotation_angle_x = 1
        rotation_angle_y = 0
        rotation_angle_z = 0

        camera = view.camera
        
        target_position = camera.target
        eye_position = camera.eye
        radius = eye_position.distanceTo(target_position)
        
        app.userInterface.messageBox(f'Cam Pos: {eye_position.x}, {eye_position.y}, {eye_position.z}')

        new_eye_position = adsk.core.Point3D.create(
            target_position.x + (eye_position.x - target_position.x) * math.cos(rotation_angle_x) - (eye_position.y - target_position.y) * math.sin(rotation_angle_x),
            target_position.y + (eye_position.x - target_position.x) * math.sin(rotation_angle_x) + (eye_position.y - target_position.y) * math.cos(rotation_angle_x),
            eye_position.z
        ) # This is a math equation I got from ChatGPT so it is probably false. I will have to do more research into what the correct function would be. 

        camera.eye = new_eye_position
        camera.isSmoothTransition = True
        view.camera = camera
        view.refresh()

    except:
        if ui:
            ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))

def run():
    global ui
    global app
    global view
    try:
        app = adsk.core.Application.get()
        view = app.activeViewport
        ui = app.userInterface
        
        move_camera()

        # Eventually this will have to catch some event via multithreading so that the script can run constantly in Fusion360 without taking all of the main thread's resources.

        #global customEvent
        #customEvent = app.registerCustomEvent(moveEvent)
        #onThreadEvent = ThreadEventHandler()
        #customEvent.add(onThreadEvent)
        #handlers.append(onThreadEvent)
    
    except:
        if ui:
            ui.messageBox('Failed:\n{}'.format(traceback.format_exc()))

run()
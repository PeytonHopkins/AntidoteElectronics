import serial, serial.tools.list_ports, ble_serial

ports = serial.tools.list_ports.comports()

for port in ports:
    print(port.name)


serialInst = serial.Serial()
serialInst.baudrate = 9600
serialInst.port = "COM8"
serialInst.open()

while 1:
    if serialInst.in_waiting > 0:
        rotation_values = serialInst.readline().decode('utf-8').strip().split(',')
        print(rotation_values)

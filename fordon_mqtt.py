import serial
import paho.mqtt.client as mqtt

serialport = 'COM4'    # Niklas får ändra
baudrate = 9600
broker = "tfe.iotwan.se"
brokerport = 1883
subscribe_topics = ["pandebil/to_car"]

def cleanup():
    ser.close()

#MQTT callbacks
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    for t in subscribe_topics:
        client.subscribe(t)

def on_message(client, userdata, msg):
    rec = msg.payload.decode()
    print("recieved message: \"{}\" from topic \"{}\"".format(rec,msg.topic))
    ser.write((rec + "\n").encode())

try:
    print("Connecting to serial port")
    ser = serial.Serial(serialport, baudrate)
    print("Connected")
except:
    print("Failed to connect to serial")
    raise SystemExit

try:
    ser.flushInput()

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set("student","Hbrygga")

    client.connect_async(broker, brokerport, 60)
        
    client.loop_start()
        
    while True:
        try:
            line = ser.readline().decode().strip()
        except:
            print("Could not decode incoming line")
            continue

        
        if line[0] != '1':
            print(f"Invalid topic identifier in line: {line}")
            continue
        print("publishing \"{}\"".format(line))
        client.publish('pandebil/from_car', line)

# handle list index error (i.e. assume no data received)
except (IndexError):
    print("No data received within serial timeout period")
    cleanup()
# handle app closure
except (KeyboardInterrupt):
    print("Shutting down")
    cleanup()
except (RuntimeError):
    print("RunTimeError occured, shutting down")
    cleanup()

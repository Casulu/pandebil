import serial
import paho.mqtt.client as mqtt

serialport = 'COM1'
baudrate = 500000
broker = "tfe.iotwan.se"
brokerport = 1883
subscribe_topics = ["pandebil/#"]

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

    topics = {0:'pandebil/to_car', 1:'pandebil/from_car', 2:'pandebil/info'}

        
    while True:
        try:
            line = ser.readline().decode().strip()
        except:
            print("Could not decode incoming line")
            continue

        
        split_line = line.split(' ', 1)
        if len(split_line) < 2:
            print(f"Incorrectly formatted line: {line}")
            continue
        topic_index = split_line[0]
        try:
            out_topic = topics[int(topic_index)]
        except:
            print(f"Invalid topic identifier in line: {line}")
            continue
        print("publishing \"{}\"".format(line))
        client.publish(out_topic, line)

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

import serial
import paho.mqtt.client as mqtt

serialport = 'COM6'
baudrate = 500000
broker = "tfe.iotwan.se"
brokerport = 1883
subscribe_topics = ["pandebil/from_car", "pandebil/to_rem1"]

def cleanup():
    ser.close()

#MQTT callbacks
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    for t in subscribe_topics:
        client.subscribe(t)

def on_message(client, userdata, msg):
    rec = msg.payload.decode('ascii', 'ignore')
    print("recieved message: \"{}\" from topic \"{}\"".format(rec,msg.topic))
    ser.write((rec + "\n").encode())

try:
    print("Connecting to serial port")
    ser = serial.Serial(serialport, baudrate)
    print("Connected")
except:
    print("Failed to connect to serial. Trying again")
    
    raise SystemExit

try:
    ser.flushInput()

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set("student","Hbrygga")

    client.connect_async(broker, brokerport, 60)
        
    client.loop_start()

    topics = {0:'pandebil/to_car', 3:'pandebil/to_rem2'}

        
    while True:
        try:
            line = ser.readline().decode().strip()
        except:
            print("Could not decode incoming line")
            continue
        if line[0] == '-':
            print("Debug: " + line)
        else:            
            try:
                out_topic = topics[int(line[0])]
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

import time
import paho.mqtt.client as mqtt

broker = "tfe.iotwan.se"
brokerport = 1883

try:
    client = mqtt.Client()
    client.username_pw_set("student","Hbrygga")

    client.connect_async(broker, brokerport, 60)
        
    client.loop_start()

    topics = {0:'pandebil/to_car', 2:'pandebil/to_rem1', 3:'pandebil/to_rem2'}

        
    while True:
        time.sleep(1)
        print("Publishing heartbeat")
        client.publish('pandebil/to_car', "00")
        client.publish('pandebil/to_rem1', "20")
        client.publish('pandebil/to_rem2', "30")

# handle list index error (i.e. assume no data received)
except (KeyboardInterrupt):
    print("Shutting down")
except (RuntimeError):
    print("RunTimeError occured, shutting down")

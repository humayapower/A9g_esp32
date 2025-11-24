import paho.mqtt.client as mqtt
import requests
import json

# ---------- Firebase Configuration ----------
# This is the Firebase Realtime Database endpoint.
# The '.json' suffix is required for REST API operations.
FIREBASE_URL = "https://espa9gnew-default-rtdb.asia-southeast1.firebasedatabase.app/mqtt_data.json"


# ---------- MQTT Callback Functions ----------
def on_connect(client, userdata, flags, rc):
    """Callback triggered when the MQTT client connects to the broker."""
    
    print("[MQTT] Connected with result code " + str(rc))
    
    # Subscribe to the topic where data will be received
    client.subscribe("test/topic")


def on_message(client, userdata, msg):
    """Callback triggered upon receiving a message from subscribed topic."""
    
    # Decode and print incoming MQTT message
    print(f"[MQTT] Message received: {msg.payload.decode()} on topic {msg.topic}")

    # Prepare formatted payload for Firebase storage
    data = {
        "message": msg.payload.decode(),   # Store received message
        "timestamp": {".sv": "timestamp"}  # Firebase auto timestamp
    }

    # Push data to Firebase using HTTP POST
    try:
        response = requests.post(FIREBASE_URL, data=json.dumps(data))
        print("[Firebase] Data pushed: ", response.text)
    except Exception as e:
        print("[Firebase] Error: ", e)


# ---------- MQTT Client Setup ----------
# Create MQTT client instance
client = mqtt.Client()

# Set Mosquitto authentication credentials
client.username_pw_set("a9guser", "anjalhum")

# Assign callback functions
client.on_connect = on_connect
client.on_message = on_message

# Connect to the local Mosquitto broker
client.connect("localhost", 1883, 60)


# ---------- Loop Forever ----------
# Keeps the script alive and continuously listening for MQTT messages
print("[System] Starting MQTT -> Firebase bridge...")
client.loop_forever()

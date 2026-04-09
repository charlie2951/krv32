import requests
import time
import random
import json
import logging
import http.client

# This "hooks" into the internal HTTP manager to show the raw data
http.client.HTTPConnection.debuglevel = 1
logging.basicConfig()
logging.getLogger().setLevel(logging.DEBUG)
requests_log = logging.getLogger("requests.packages.urllib3")
requests_log.setLevel(logging.DEBUG)
requests_log.propagate = True

# --- Configuration ---
USERNAME = 'skmaityfet'
API_KEY = 'aio_QSJp66LcMFWo1nCSQr4gw0Vdhi44'
FEED_KEY = 'temperature'

# Note the 'http' instead of 'https'
url = f"http://io.adafruit.com/api/v2/{USERNAME}/feeds/{FEED_KEY}/data"

headers = {
    'X-AIO-Key': API_KEY,
    'Content-Type': 'application/json'
}

print(f"Target URL: {url}")
print("-" * 30)

while True:
    # 1. Generate Data
    temp_value = random.randint(10, 50)
    
    # 2. Create Payload (The Dictionary)
    payload_dict = {'value': temp_value}
    
    # 3. Convert to JSON String (This is what actually goes over the wire)
    json_payload = json.dumps(payload_dict)
    
    # --- DEBUG PRINT: Show exactly what is being sent ---
    print(f"DEBUG: Sending Payload -> {json_payload}")
    print(f"DEBUG: Content-Length -> {len(json_payload)}")
    
    try:
        # 4. Perform the POST request
        response = requests.post(url, data=json_payload, headers=headers)
        
        # 5. Check response
        if response.status_code in [200, 201]:
            print(f"SUCCESS: Data {temp_value} received by Adafruit.")
        else:
            print(f"ERROR: {response.status_code} - {response.text}")
            
    except Exception as e:
        print(f"Network Error: {e}")

    print("-" * 30)
    time.sleep(10) # Wait 10 seconds
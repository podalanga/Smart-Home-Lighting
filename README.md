# Smart Home Lighting

Are you tired of just learning web development? Do you want to implement an asynchronous server using affordable hardware? __Look no further!__

Smart Home Lighting, our coursework project turned repository, is here to help you understand the integration of embedded systems and web development using Wi-Fi and HTTP. This project replicates the functionality of Philips Hue, featuring a main switch to control the light, a software switch on a website, an ambient mode to adjust LED brightness based on ambient light, and an HSV color wheel to change the LED color.

The board used for this project is the NodeMCU32s, which is more widely available than the original ESP32s. Let’s first understand the basic schematic of the setup:

![](Attachments/Pasted%20image%2020250420012011.png)

The peripherals include a main switch, an LDR (Light Dependent Resistor), and an RGB LED.

![](Attachments/Pasted%20image%2020250420012828.png)

__NOTE: We didn’t have a physical switch, so we used jumper wires as switches.__

## Components Used
- NodeMCU32s
- Breadboard
- Common Emitter (CE) RGB LED
- 100-ohm resistor
- 10k-ohm resistor
- 1 LDR
- Switch (latching, not push-button)
- Jumper wires
- Multimeter (for debugging)

## Circuit Connections
1. Place the NodeMCU32s on the breadboard and connect the 3.3V and GND pins to the power rails of the breadboard for easier connections.
2. Connect the 3.3V rail to one terminal of the LDR and the other terminal to a 10k-ohm resistor. Connect a jumper wire from pin 36 to the junction between the LDR and the resistor. This will serve as the ADC input for ambient mode.
3. Connect the RGB LED pins: pin 32 for red, pin 33 for green, and pin 25 for blue. Attach a 100-ohm resistor to the common emitter of the LED and connect it to GND.
4. Connect one terminal of the main switch to the power rail and the other terminal to pin 22.

## Algorithm Overview
![](Attachments/Pasted%20image%2020250420012754.png)

The HSV color values are fetched from the HSV color wheel and processed by the NodeMCU32s.

![](Attachments/Pasted%20image%2020250420013119.png)

### **Pseudo Code**

#### **Initialization**
- Begin UART communication at 115200 baud.
- Initialize the SPIFFS file system.
- Connect to Wi-Fi using the provided SSID and password.
- While not connected:
    - Print `"Connecting to Wi-Fi..."`.
- If mDNS starts successfully:
    - Allow access via `http://<user-defined-name>.local`.
- Setup PWM for RGB LEDs on respective GPIO pins with appropriate PWM channels and frequency.
- Set pin modes for the main switch and LDR sensor.

#### **Web Server Setup**
- Start the server on port 80.
- Serve `index.html` when `/` is requested.
- Handle `"/sethsv"` requests with HSV parameters:
    - Store the HSV values from the URL query.
- Handle `"/led"` requests with `"state"`:
    - Update the LED on/off state.
- Handle `"/ambient"` requests with `"state"`:
    - Update the ambient mode state.

#### **Main Loop**
- Read the LDR sensor value.
- If the main switch is ON and the LED state is ON:
    - If ambient mode is ON and the LDR reading indicates low light:
        - Calculate brightness based on ambient light.
        - Convert HSV to RGB using the calculated brightness.
    - Otherwise:
        - Convert HSV to RGB using the current values from the website.
        - Set the RGB LED brightness using PWM.
- Otherwise:
    - Turn off all LEDs.

#### **Helper Functions**
1. **HSV2RGB(h, s, v)**:
     - Convert HSV values to corresponding RGB values.
     - Scale RGB values to 8-bit (0–255) PWM output.
2. **check_switch()**:
     - Read the digital input from the main switch pin.
     - Return `1` if ON, `0` if OFF.
3. **init()**:
     - Set up PWM channels and attach them to RGB pins.
     - Set pin modes for the main switch and LDR.

__Note: Update the `ssid` and `password` variables in the code to match your Wi-Fi credentials (mandatory). You can also customize the DNS name parameter, but this is optional.__

## How to Upload the Code
We recommend using PlatformIO, a robust development environment for embedded systems, which offers more features than the Arduino IDE.

1. Import this project folder into VS Code.
2. Open PlatformIO and access the command window.
3. Connect the NodeMCU32s to your computer and run:
     ```bash
     pio run --target uploadfs
     ```
     This uploads the HTML code to the virtual memory created using SPIFFS.
4. Build and upload the C++ code by clicking the build button.

## How to Access the Asynchronous Server
The IP address of your NodeMCU will be displayed in the serial monitor. Paste it into your browser to fetch the HTML code from the server. Alternatively, use the domain:
```
podalanga.local
```
__Note: If you changed the DNS name in the code, use `<your_custom_name>.local` instead.__

Ensure the main switch is ON and the LED soft switch in the browser is enabled. Use the color wheel to change the LED colors and test the ambient mode under various lighting conditions.

***That’s it! You’re all set to explore and enjoy your Smart Home Lighting project!***

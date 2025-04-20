# Smart Home Lighting

Tired of just learning Web development? Wanted to implement an Asynchronous server with anything cheap that can host it? __Fret not!!!__

Smart Home Lighting: our Coursework project turned repository could help you to understand the basic integration of embedded systems and Web Development using WIFI and HTTP. The implementation was to replicate what Philips Hue actually does right now. So a main switch to control the complete option of turn on and off. A software switch in website, ambient mode for determining the brightness of ambient light and changing the LED brightness. Last but not the least, HSV circle to change the colour of LED.

The board I had used was NodeMCU32s which was widely available than ESP32s original. Let us first understand the basic schematic of the current setup.
![](Attachments/Pasted%20image%2020250420012011.png)

The peripherals are main switch, LDR and LED in this given circuit.
![](Attachments/Pasted%20image%2020250420012828.png)

__NOTE: we didnt have a switch so we used jumpers as switches itself.__
## Connections made
We had used
- NodeMCU32s
- Breadboard
- CE(Common Emitter) RGB LED
- 100ohm resistor
- 10k ohm resistor
- 1 LDR
- switch which can latch to previous state (not push button)
- Bunch of Jumper wires
- Multimeter (debugging)

Now let us look into the steps of implementing
1. Place the NodeMCU32s on the breadboard properly and connect 3.3V and Ground to the parallel lines of breadboard for further easier connections.
2. Connect 3.3V line to LDR and then with 10k ohm resistor. Connect a jumper from pin 36 to the junction between them. This will be your ADC reading pin for Ambient mode.
3. Connect 32 for red pin, 33 for green pin and 25 for blue pin of LED. Then connect a 100ohm resistor to the Common emitter of the LED which is then grounded.
4. Connect main switch's first terminal to power and the second terminal to pin 22
Now lets look into the algorithm,
![](Attachments/Pasted%20image%2020250420012754.png)

We get values from this HSV circle and that gets fetched by the NodeMCU32s.

![](Attachments/Pasted%20image%2020250420013119.png)

### **Pseudo Code**

#### **Initialization**

- Begin UART communication at 115200 baud
    
- Initialize SPIFFS file system
    
- Connect to Wi-Fi using given SSID and password
    
- While not connected:
    
    - Wait and print `"Connecting to Wi-Fi..."`
        
- If mDNS starts successfully:
    
    - Allow access via `http://<user defined name>.local`
        
- Setup PWM for RGB LEDs on respective GPIO pins with respective PWM channels and frequency
    
- Set pin modes for main switch and LDR sensor

#### **Web Server Setup**

- Start server at port 80
    
- Serve `index.html` if present when `/` is requested
    
- When `"/sethsv"` is requested with **HSV** parameters:
    
    - Store the HSV values from the URL query
        
- When `"/led"` is requested with `"state"`:
    
    - Update LED on/off state
        
- When `"/ambient"` is requested with `"state"`:
    
    - Update ambient mode state
#### **Main Loop**

- Read **LDR** sensor value
    
- If main switch is ON and LED state is ON:
    
    - If ambient mode is ON and **LDR** reading indicates low light:
        
        - Calculate brightness based on ambient light
            
        - Convert **HSV** to **RGB** using calculated brightness
            
    - Else:
        
        - Convert **HSV** to **RGB** using current values in the website
            
        - Set **RGB** LED brightness using **PWM**
            
- Else:
    
    - Turn off all **LEDs**
#### **Helper Functions**

**1.) HSV2RGB(h, s, v)**

- Convert **HSV** values to corresponding **RGB**
    
- Scale **RGB** to 8-bit (0–255) **PWM** output
    

**2.) check_switch()**

- Read digital input from main switch pin
    
- Return `1` if ON, `0` if OFF
    

**3.) init()**

- Set up **PWM** channels and attach them to **RGB** pins
    
- Set pin modes for main switch and **LDR**

__Note: Change the ssid and password variable in the code with respect to your wifi router's wifi name and password (Mandatory).  The code also has dns name parameter, you can change it to however you want but this is optional__
## How to upload the code
I had used Platform IO which is a full blown working environment for Embedded stuffs. It has great functionalities compared to Arduino IDE.
After importing this folder in your VS code, open up Platform IO and then open the Command window for platformio. 
While the board has been connected to the computer,
```pio
pio run --target uploadfs
```
This will upload the html code into the Virtual memory we created using SPIFFS. Now then again upload again using the build button. This is to upload the normal C++ code we got.

## How to access the Asynchronous Server?
The IP address of your NodeMCU would be displayed in your serial monitor, paste it in the browser and use it to fetch the HTML code from the server. If you couldn't find it, use the Domain
```
podalanga.local
```
__Note: use this if you hadn't changed the code for DNS. Else, <your_name_in_code>.local would work.__
Ensure the main switch is on and LED soft switch in the browser is on. Then press the colour wheel to change the colours. Also try out the ambient mode in various light conditions.

***That is it. You are now good to go!!!***

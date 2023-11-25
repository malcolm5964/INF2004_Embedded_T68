# Data Flow Diagram
![Embedded Diagram](https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/89ea4c7e-99b6-40d1-9032-8a09c578d9ef)
- Ultrasonic, Magnometer, Wheel Encoder and IR Sensor only needs to get data for Mapping.c and Barcode.c to perform logic and calculations.
- Mapping.c will return which direction and distance the car should move next with the Heading Degree data and the IR Sensor data to detect if there is a wall or not.
- Barcode.c will decode the data gathered from IR Sensor to an Alphabet
- Wifi Server will request for the Map Data and decoded barcode information and display it with the help of ssi and cgi handlers.



# Flow Chart

1. Motor Controller
<p align="center">
  <img width="141" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/69d18d03-363d-4fbd-9aa3-d35b0db7920d">
  <p>Flow of motor controller and actions to make the car straight</p>
</p>

3. Wheel Encoder
<p align="center">
<img width="190" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/3d8e0ae1-ba55-450a-a786-a6098eb8c14d">
<p>Flow of Wheel Encoder and steps to calculate distance and speed</p>
</p>

5. Magnometer
<p align="center">
<img width="275" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/0980a073-e760-46de-9d8e-355d84518202">
</p>





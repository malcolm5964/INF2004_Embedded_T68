# Block Diagram
![Embedded Diagram (1)](https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/462bd128-cb81-4fe6-9310-f2eafe0ffd90)
- Blue Rectangle = Hardware Compoenents
- Green Circle = Software Components

# Data Flow Diagram
![Embedded Diagram](https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/89ea4c7e-99b6-40d1-9032-8a09c578d9ef)
- Ultrasonic, Magnometer, Wheel Encoder and IR Sensor only needs to get data for Mapping.c and Barcode.c to perform logic and calculations.
- Mapping.c will return which direction and distance the car should move next with the Heading Degree data and the IR Sensor data to detect if there is a wall or not.
- Motor.c requires information to decide the car movement
  - Information from Mapping.c will be pass to decide which direction the wheel should turn and distance to move.
  - Information from wheelEncoder.c to decide when to stop if the distance is reach or control the speed(PID)
  - Information from Magnometer.c to decide when to stop turning.
- Barcode.c will decode the data gathered from IR Sensor to an Alphabet
- Wifi Server will request for the Map Data and decoded barcode information and display it with the help of ssi and cgi handlers.




# Flow Chart

1. Motor Controller
<p align="center">
  <img width="141" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/69d18d03-363d-4fbd-9aa3-d35b0db7920d">
  <p>Flow of motor controller and actions to make the car straight</p>
</p>

2. Wheel Encoder
<p align="center">
<img width="190" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/3d8e0ae1-ba55-450a-a786-a6098eb8c14d">
<p>Flow of Wheel Encoder and steps to calculate distance and speed</p>
</p>

2. Magnometer
<p align="center">
<img width="275" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/0980a073-e760-46de-9d8e-355d84518202">
<p>Flow of Magnometer and how it turn 90 degree</p>
</p>

3. Ultrasonic
<p align="center">
<img width="172" alt="Screenshot 2023-11-25 234754" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/4eba62cc-67ce-4f8c-99e1-28b21faabd2c">
<p>Flow of ultrasonic on how it detects an obstacle</p>
</p>

# Mapping Algorithm

1. Create an Adjacency for each Vertex to store vertices connected to it.
Vertex 1:
Vertex 2:
Vertex 3:
Vertex 4:
Vertex 5:
.
.
.
Vertex 36:

2. Check Vertex 1
<img width="887" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/f66ce54b-d37c-4671-ba49-763be73d24e4">


3. Store the wall readings into the adjacency list
Vertex 0: None
Vertex 1: 2 -> 7 -> 31
Vertex 2: 1
Vertex 7: 1
Vertex 31: 1

4. Go through adjacency list to find next vertex to check
Next vertex is 2.
Position car at vertex 2 and run CheckVertex().

5. Repeat for all vertices, Car will return back to start
<img width="826" alt="image" src="https://github.com/malcolm5964/INF2004_Embedded_T68/assets/25952539/d3e741af-9717-4eac-8c77-cc880b222354">






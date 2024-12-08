# Final Project
## Introduction:
The **Smart Traffic System Stimulator** is a project that recreates urban traffic management system while focusing on traffic flow in real-time. The model works with data structures like Graph, Queue, Linked list and hash table to create a sysytem that enables efficient vehicle routing, adaptive traffic signal control, congestion monitoring and emergency handling. The stimulator ensures smooth traffic flow by responding to changes in traffic conditions and disruptions like accidents and road closures.With interactive features and robust algorithm such as  Dijkstra's, A* and BFS. The project offers a practical appraoch to understanding and managing city traffic systems.

## Features:
### 1. City Traffic Network:
* A Vertix class that represents the intersections.
* An Edge class to represent roads, with weight indicating travel time.
* Supports adding and checking vertices and edges `add_vertex`, `add_edge` and `vertex_exists`.
* Tracks and updatesw road status dynamically `change_road_status`.

### 2. Vehicle Routing System:
* Calculate the shortest path for vehicle using:
   * Dijkstra’s Algorithm(`dijkstra`)
   * A Algorithm* for heuristic-based routing (`a_star`,`add_vertex`).
* Supports vehicle and emergency vehicle addition ( `add_vehicle`,`add_emergency_vehicle`).
* Finds and assigns path to all vehicles(`find_path_for_all`).
* Allows path reversal for vehicles when needed(`reverse_vehicle_path`).
  
### 3. Traffic Signal Management:
* Manages traffic using a queue:
   * Keeps a queue of intersection for green signals(`initialize_queue`,`green_queue`).
   * Dynamically adjusts signal duration based on traffic (`set_time`,`green_light`).
   * Includes features for monitoring and priniting the state of the green queue(`green_queue.check`, `green_queue.print_queue`).

### 4. Congestion Monitoring:
* Uses hash table to:
    * Track vehicle counts on each road segment(`get_VehicleCount`).
    * Detect congestion on roads(`Congestion`).
* Monitors vehicles at intersections and prints their details(`print_vehicles_at_vertex`).
### 5. Emergency Vehicle Handling
* Finds the fastest path using A* Algorithm(`emergency_path`).
* Temporarily prioritizes emergency vehicles by overriding normal routing.
* Reroutes vehicles to clear the path for emergency vehicles(`emergency_rerouting`).
  
### 6. Accident and Road Closure Simulation:
* Blocks specific roads dynamically (`change_road_status`) in hash table.
* Recalculates paths for affected vehicles when roads statuses change(`find_path_for_all`).
* Updates road conditions and handles disruption effectively.

### 7. Simulation Dashboard:
* Supports loading data from external files:
  * Road Network(`read_file`).
  * Traffic Signals(`read_signals`).
  * Vehicle and emergency vehicle information(`read_vehicles`,`read_emergency_vehicles`).
* Display vehicle routing, signal states and congestion levels through print functions.
* Offers a manual menu-driven appraoch for testing emergency vehicle rerouting.
  
## Instructions:
* The user chooses option from the Menu.
  ### MENU:
![Screenshot 2024-12-08 164835](https://github.com/user-attachments/assets/83d1ff80-90b0-4c4f-8d59-80986634045a)

* If the user enters '1', Traffic Network is displayed:
### Traffic Network:
![traffic_network_display](https://github.com/user-attachments/assets/0f48eaed-8375-4bb7-8db1-df799e55453b)

* If the user enters '2', Congestion Level is displayed:
### Congestion Level:
![NO_CONGESTION](https://github.com/user-attachments/assets/c81525a5-8a68-4a52-bb1d-1c10bb545432)

* If the user enters '3', Blocked Roads are displayed:
### Blocked Roads:
![blocked roads](https://github.com/user-attachments/assets/bac38e35-dd59-4030-ba73-78d5fedb726f)

* If the user enters '4', Vehicles are displayed:
### Vehicle display:
![vehicle_display](https://github.com/user-attachments/assets/a9a3e676-ed72-47cc-91ce-6cfd793811aa)

* If the user enters '5', Vehicles routes are displayed:
### Vehicle Routes:
![vehicle_route](https://github.com/user-attachments/assets/4e3114e3-4f7f-4cdb-8900-9e3665c3053c)

* If the user enters '6', Emergency Vehicle Routing are displayed:
### Emergency Vehicle Routing:
![vehicle_routing](https://github.com/user-attachments/assets/f4ddc1b2-1e89-4953-a119-e5bba51ccccd)

* If the user enters '7', Block Roads are displayed:
### Block a Road:
![ooption 7](https://github.com/user-attachments/assets/48189de9-8617-4b7a-9cfb-bb4cd8e64ef8)

* If the user enters '8', it Simulates Vehicle Routing:
### Unblock a Road:
![option 8](https://github.com/user-attachments/assets/9b4624c4-c7e2-4013-869d-018b84344f80)


* If the user enters '9', it Simulates Vehicle Routing:
### Stimulate Vehicle Routing:
### Vehicle Routing:
![vehicle_routing_queue](https://github.com/user-attachments/assets/88d7aa3e-7a6b-4f0b-a652-9644f8cbc06c)


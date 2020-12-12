# 1 Problem Statement

This algorithm is suitable for solving the single-vehicle VRP problem with
pickup and delivery with time window (VRPPDTW) based on historical daily travel
time data, using Lagrangian relaxation algorithm and dynamic programming
algorithm. The model and algorithm are sepcified in my thesis, which could be
cited after July 2021.

**Input:** physical network, passenger information(original, destination and
time window)and vehicle information(capacity and time window)

**Output:** space-time path of vehicle and state transition

**Constraints:** time window constraints, vehicle capacity constraints,
passenger demands constraints and unique physical path constraints

**Objective:** the least-expected cost

## 1.1 Vehicle Routing Problem(VRP)

The VRP problem was first proposed by G. Dantzig and J. Ramser in 1959. The
traditional VRP problem can be summarized as: a fleet provides services for a
certain number of passengers, and we need to find a series of vehicle routes(
Starting from the parking lot, returning to the parking lot after completing the
service for passengers) under vehicle capacity constraints, to achieve the
objective of shortest distance, or the least cost, or the least travel time.

The actual vehicle routing problems include distribution, bus routing, letter
and newspaper delivery, air and railway schedule arrangements, etc.

## 1.2 Vehicle Routing Problem with Time Window(VRPTW)

When all passenger services need to be carried out in a given time interval
(called time window), it is a VRP problem with a time window(VRPTW), an
extension of the general VRP problem. The time window can be regarded as a soft
constraint or a hard constraint. When all services must be performed within a
given time window, it is a hard constraint; when a penalty value is imposed on
services outside the time window, it is a soft constraint.

## 1.3 Vehicle Routing Problem with Pickup and Delivery(VRPPD)

The traditional VRP problem only considers a single process of "gathering" or
"distributing" people or goods. During the course of a vehicle, the change in
passenger/cargo volume is monotonous. The VRP problem with pickup and delivery
(VRPPD) is a collection of pickup and delivery services. It needs to be carried
out at the designated starting point and endpoint according to customers' needs.
It also considers the pickup and drop-off of passengers or the collection and
distribution of goods in route planning.

## 1.4 Vehicle Routing Problem with Pickup and Delivery with Time Window(VRPPDTW)

The VRPPDTW is a problem that all passengers or goods need to be picked up
within a specified time window and delivered to the destination within another
fixed time window. At the same time, each vehicle also has the time window
constraints for leaving and returning to the warehouse and capacity constraints.

# 2 Underlying Network Structure

## 2.1 Nodes of VRPPDTW Network

In the VRPPDTW network, there are physical nodes, passenger pickup and delivery
nodes, and vehicle station nodes. Take a six-node physical network of case 1 in
**Figure1** as an example. The original of passenger 1 is at node 2 and the
destination is at node 6; the original of passenger 2 is at node 5, and the
destination is at node 3. There is a vehicle at the warehouse of node 4
distributed to serve the two passengers, and the vehicle finally returns to the
warehouse of node 1. In order to distinguish between physical nodes, passenger
pickup and delivery nodes, and vehicle station nodes, this paper respectively
adds virtual nodes (the original of passenger 1) and (the destination of
passenger 1) at node 2 and node 6, (the original of passenger 2) and (the
destination of passenger 2) at node 5 and node 3, (the departure warehouse of
vehicle 1) and (the return warehouse of vehicle 1) at node 4 and node 1.

The time window is indicated in parentheses next to the virtual node. For
example, passenger 1 must be picked up at the virtual node within the time
window [5,7], and delivered at the virtual node within [9,12]; passenger 2 must
be picked up at the virtual node within [8,10], and delivered at the virtual
node within the time window [14,19]. Similarly, the vehicle must depart from the
warehouse at t=1 and return to the warehouse within [1,30].

![](media/57646e423192da3c4d47eeecd40e84df.png)

>   图片包含 游戏机, 物体, 钟表 描述已自动生成

**Figure 1 Six-node network with virtual nodes**

## 2.2 Arcs of VRPPDTW Network

According to different types of nodes, arcs can also be divided into three
categories: transportation arcs, service arcs and waiting arcs, shown in
**Figure 2**.

1.  Transportation arcs

A transportation arc connects two adjacent transportation nodes, which means
that the vehicle is travelling on the corresponding physical link. The state of
passengers and vehicles passing through the transportation arc remains
unchanged.

1.  Service arcs

Service arc can be pickup arc or delivery arc, which means that passenger gets
on or off the vehicle. Two nodes linked by the service arc are the receiving
node (the original of the passenger) and the sending node (the destination of
the passenger). Obviously, the state of the vehicle and passengers will change
due to the behaviour of getting on and off the vehicle.

1.  Waiting arcs

Waiting arcs indicate that the vehicle stops and waits to satisfy the time
window constraints. For example, the green horizontal line in **Figure 2**
indicates that the vehicle at is waiting for passenger 1. Once passenger 1
approaches the location, the vehicle can pick him/her up immediately.

![图片包含 游戏机 描述已自动生成](media/7e26fec7a41e95ff2f41eab527fa4abc.png)

**Figure 2 Space-time arcs**

## 2.3 States Transition

1.  States representation

In the time-space-state network constructed in this paper, the state refers to
the cumulative service times of passengers. The passenger state can also be
expressed as a vector [ with elements, where the value of can be 0, or 1, or 2,
respectively indicating that the vehicle serves passengers 0 times (unserved
passengers), or the vehicle serves Passengers 1 time (the vehicle picks up
passengers), or the vehicle serves passengers 2 times (the vehicle drops
passengers). As shown in **Figure 3**, in order to facilitate the description of
the state transition, we introduce the following equivalent symbol system for
the cumulative service states of passengers: if the vehicle carries passenger ,
then the -th element of state fills the passenger ID; otherwise, it will be
filled with a dotted line. **Figure 3** shows the state of carpooling: [(no
passengers are served), [( passenger 1 is picked up), [(passenger 2 is picked
up), [(passenger 1 is delivered) and [(passenger 2 is delivered). The
accumulated service states of passengers corresponding to these states are shown
in **Table 1**.

![图片包含 游戏机, 物体, 钟表 描述已自动生成](media/ed7730e6be8d3c381592883df2fc8573.tiff)

>   **Figure 3 Equivalent symbol of states transition**

![图片包含 游戏机, 物体, 钟表 描述已自动生成](media/ed7730e6be8d3c381592883df2fc8573.tiff)

>   **Figure 3 Equivalent symbol of states transition**

>   **Table 1 Accumulated service states of passengers and equivalent symbol**

| accumulated service states representation | equivalent symbol representation |
|-------------------------------------------|----------------------------------|
| [                                         | [(no passengers are served)      |
| [1                                        | [ ( passenger 1 is picked up)    |
| [1                                        | [ (passenger 2 is picked up)     |
| [2                                        | [ (passenger 1 is delivered)     |
| [2                                        | [ (passenger 2 is delivered)     |

1.  States transition

Initial state means the empty state of each vehicle without any passengers. Each
vertex in the time-space-state network is represented and identified by node ,
time interval and passenger state . Node is connected to the node through arc .
To find all possible state transitions on the arc, just follow the following
rules:

**Rule 1**: On a pickup link(a link between the physical node and virtual
original node of the passenger), vehicle picks up passenger , so changes from 0
to 1, or the -th element of equivalent state changes from the dotted line to the
ID of passenger .

**Rule 2**: On a delivery link(a link between the virtual destination node of
the passenger and physical node), vehicle delivers passenger , so changes from 1
to 2, or the -th element of equivalent state changes from ID of passenger to the
dotted line.

**Rule 3**: On a transportation link or a link in which one node is the virtual
node of vehicle, vehicle will neither picks up nor delivers any passenger, so
all elements of and should be the same.

To find all feasible state transitions , we need to check all possible
combinations of and . Considering a three-passenger case, **Table 2** lists all
possible combinations of these state transitions. Please note that in this
example, the vehicle can carry up to two passengers simultaneously. Empty cells
represent state transitions that are not feasible. **Figure 4** shows the
corresponding possible passenger-carrying state transitions. The green arrow
indicates pickup, and the blue arrow indicates delivery.

>   **Table 2 All possible state transitions**

|   | [         | [         | [         | [         | [         | [         | [         |
|---|-----------|-----------|-----------|-----------|-----------|-----------|-----------|
| [ | no change | pickup    | pickup    | pickup    |           |           |           |
| [ | delivery  | no change |           |           | pickup    | pickup    |           |
| [ | delivery  |           | no change |           | pickup    |           | pickup    |
| [ | delivery  |           |           | no change |           | pickup    | pickup    |
| [ |           | delivery  | delivery  |           | no change |           |           |
| [ |           | delivery  |           | delivery  |           | no change |           |
| [ |           |           | delivery  | delivery  |           |           | no change |

![图示 描述已自动生成](media/824a8e9d75b3428e461e766c06c3aa51.tiff)

>   **Figure 4 All possible state transitions**

![图示 描述已自动生成](media/824a8e9d75b3428e461e766c06c3aa51.tiff)

>   **Figure 4 All possible state transitions**

Taking a six-node network (**Figure 1**) as an example, **Figure 5** is the
equivalent state transition representation diagram, and **Figure 6** is the
cumulative service state transition representation diagram.

![地图的截图 描述已自动生成](media/19f257a161bdf0dec8558fb65f649278.tiff)

**Figure 5 Equivalent state transition representation diagram**

![地图的截图 描述已自动生成](media/82ad7cef4a2c44fb4dc53ec5df33b875.tiff)

>   **Figure 6 Cumulative service state transition representation diagram**

# 3 Methodology

>   **Table 3 Variable symbol directory**

| Variable | Definition                                                                                                     |
|----------|----------------------------------------------------------------------------------------------------------------|
|          | nodes                                                                                                          |
|          | time                                                                                                           |
|          | states                                                                                                         |
| ,        | node's space, time, and status indicators                                                                      |
|          | time-space-state arc, representing the change from node to node , from time to time , and from state to state  |
|          | on day , if the vehicle passes through an arc , then the value of this variable is 1. Otherwise, it is 0       |

The mathematical model is shown in the following function, and the objective
function is the least-expected transportation cost. Constraints are composed of
flow balance constraints, passenger demands constraints, unique physical path
constraints, and 0-1 variable constraint.

|   |        |   |
|---|--------|---|
|   | *s.t*  |   |
|   |        |   |
|   |        |   |
|   |        |   |
|   |        |   |
|   |        |   |
|   |        |   |
|   | ,      |   |

In order to solve the model efficiently, the Lagrangian relaxation algorithm is
used to transform the passenger demands constraints and the unique physical path
constraints into objective function terms. Therefore, remaining constraints and
the new objective function become the shortest path problem related to time. By
continuously updating the Lagrangian multiplier, the dynamic programming method
is used to solve sub-problems, and the model is solved iteratively. The new
model is shown below.

|   |        |   |
|---|--------|---|
|   | *s.t*  |   |
|   |        |   |
|   |        |   |
|   |        |   |
|   |        |   |
|   | ,      |   |

# 4 Quick Run

1.  Method 1

Double-click AgentPlus.sln, then right-click AgentPlus project, select
Regenerate, and find the path of AgentPlus.exe shown in **Figure 7**. Put input
files into the debug folder and double-click the AgentPlus.exe to run the
program.

![文本 描述已自动生成](media/024ca15fd047943c6b46118b95a3e6b3.png)

**Figure 7 Find the path of AgentPlus.exe**

1.  Method 2

Copy AgentPlus.exe to the folder where input files are located and double-click
the .exe file to run the program.

# 5 Input and Output

The following data set uses case1 as an example to describe the input and output
data in detail.

## 5.1 Input

1.  input_node.csv

As shown in **Table 4**, input_node.csv is the input file of physical nodes
information, including: node_id, x, y. node_id is the number of the physical
node. x,y are the horizontal and vertical coordinates of nodes and the x,y
coordinates of nodes can be used to calculate link length.

**Table 4 input_node.csv**

| node_id | x      | y      |
|---------|--------|--------|
| 1       | 61.671 | 71.431 |
| 2       | 60.451 | 70.882 |
| 3       | 62.22  | 69.113 |
| 4       | 60.939 | 68.198 |
| 5       | 62.83  | 66.063 |
| 6       | 58.194 | 69.967 |

1.  input_link.csv

As shown in **Table 5**, input_link.csv is the input file of physical link
information. The main content included are:

a. from_node_id: original of the link;

b. to_node_id: destination of link;

c. direction: direction of link. 1 is the positive direction
(from_node→to_node), -1 is the reverse direction (to_node→from_node);

d. travel_time_1: link travel time of day1;

e. travel_time_2: link travel time of day2;

The remaining parameters include: speed_limit_in_mph, number_of_lanes,
lane_capacity_in_vhc_per_hour, jam_density. This program has not been
considered, and users can expand according to actual needs.

1.  input_agent.csv

As shown in **Table 6**, input_agent.csv takes passengers and vehicles
information as inputs.

a. agent_id: the number of passenger/vehicle (numbered separately);

b. agent_type: 0 means passenger, 1 means vehicle;

c. from_node_id, to_node_id: original and destination;

d. departure_time_start, departure_time_window: earliest departure time and
length of time window;

e. arrival_time_start, arrival_time_window: the earliest arrival time and length
of time window;

f. capacity: vehicle capacity;

g. base_profit: the initial price of passengers;

Other parameters include VOIVTT_per_hour and VOWT_per_hour. Due to this program
has not yet considered, users can expand according to actual needs.

**Table 5 input_link.csv**

| from_node_id | to_node_id | direction | travel_time_1 | travel_time_2 |
|--------------|------------|-----------|---------------|---------------|
| 1            | 2          | 1         | 2             | 2             |
| 1            | 3          | 1         | 2             | 2             |
| 1            | 5          | 1         | 1             | 1             |
| 2            | 1          | 1         | 2             | 2             |
| 2            | 4          | 1         | 2             | 2             |
| 2            | 5          | 1         | 1             | 1             |
| 3            | 1          | 1         | 2             | 2             |
| 3            | 4          | 1         | 2             | 2             |
| 4            | 2          | 1         | 2             | 2             |
| 4            | 3          | 1         | 2             | 2             |
| 5            | 6          | 1         | 1             | 1             |
| 6            | 3          | 1         | 1             | **6**         |
| 6            | 4          | 1         | 1             | 1             |

**Table 6 input_agent.csv**

| agent_id | agent_type | from_node_id | to_node_id | departure_time_start | departure_time_window | arrival_time_start | arrival_time\_ window | capacity | base_profit |
|----------|------------|--------------|------------|----------------------|-----------------------|--------------------|-----------------------|----------|-------------|
| 1        | 0          | 2            | 6          | 5                    | 2                     | 9                  | 3                     |          | 6           |
| 2        | 0          | 5            | 3          | 8                    | 2                     | 14                 | 5                     |          | 6           |
| 1        | 1          | 4            | 1          | 1                    | 29                    | 1                  | 29                    | 2        |             |

Through inputting the above three files, the program can construct a virtual
network including virtual nodes and virtual arcs, as shown in **Figure 8**
below.

![图片包含 游戏机, 物体, 钟表 描述已自动生成](media/36b758924feeb4387bea1190d8c9e3e2.png)

![图片包含 游戏机, 物体, 钟表 描述已自动生成](media/ea71f5ad78e6ec92e62da5d514e0cfc2.png)

(a) (b)

Figure 8 Virtual network of case1

![图片包含 游戏机, 物体, 钟表 描述已自动生成](media/36b758924feeb4387bea1190d8c9e3e2.png)

![图片包含 游戏机, 物体, 钟表 描述已自动生成](media/ea71f5ad78e6ec92e62da5d514e0cfc2.png)

(a) (b)

Figure 8 Virtual network of case1

## 5.2 Output

1.  output_node.csv

According to the input_node.csv and input_agent.csv files, the program adds
virtual nodes: passenger pickup and delivery nodes and vehicle station nodes.

a. node_id: node number. The numbering sequence is physical node, passenger 1
pickup node, passenger 1 delivery node, passenger 2 pickup node, passenger 2
delivery node,..., vehicle departure node, vehicle arrival node.

b. node_type: node attribute. 0 represents the physical node, 1 represents the
passenger pickup node, 2 represents the passenger delivery node, and 3
represents the departure and arrival node of the vehicle.

According to the previous input data, we get output_node.csv, as shown in
**Table 7**. Among node_id, 1-6 are physical nodes; 7-8 are pickup and delivery
nodes of passenger 1, corresponding to and in **Figure 8**; and 9-10 are pickup
and delivery nodes of passenger 2, corresponding to and ; 11-12 correspond to
and .

1.  output_link.csv

In the same way, in addition to the existing physical input links, the program
will construct virtual arcs based on input_link.csv and input_agent.csv, and
output the output_link.csv file shown in **Table 8**.

a. from_node_id and to_node_id: respectively represent the starting and ending
point of arc.

b. link_type: 0 represents the physical arc, 1 represents the pickup arc of
passenger 1, which is the arc from node 2 to in **Figure 8**, and -1 represents
the delivery arc of passenger 1, which is the arc from node 3 to . In the same
way, the arcs with attributes 2 and -2 represent pickup and delivery arcs of
passenger 2, respectively. 100 represents a vehicle departure arc, which is the
arc from to node 4, and 101 represents a vehicle return arc, which is the arc
from node 1 to .

Note: The program sets the travel time of virtual arcs to 1, which can be
modified as required.

**Table 7 output_node.csv**

| node_id | node_type |
|---------|-----------|
| 1       | 0         |
| 2       | 0         |
| 3       | 0         |
| 4       | 0         |
| 5       | 0         |
| 6       | 0         |
| 7       | 1         |
| 8       | 2         |
| 9       | 1         |
| 10      | 2         |
| 11      | 3         |
| 12      | 3         |

**Table 8 output_link.csv**

| from_node_id | to_node_id | link_type |
|--------------|------------|-----------|
| 1            | 2          | 0         |
| 1            | 3          | 0         |
| 1            | 5          | 0         |
| 2            | 1          | 0         |
| 2            | 4          | 0         |
| 2            | 5          | 0         |
| 3            | 1          | 0         |
| 3            | 4          | 0         |
| 4            | 2          | 0         |
| 4            | 3          | 0         |
| 5            | 6          | 0         |
| 6            | 3          | 0         |
| 6            | 4          | 0         |
| 2            | 7          | 1         |
| 7            | 2          | 1         |
| 6            | 8          | \-1       |
| 8            | 6          | \-1       |
| 5            | 9          | 2         |
| 9            | 5          | 2         |
| 3            | 10         | \-2       |
| 10           | 3          | \-2       |
| 11           | 4          | 100       |
| 1            | 12         | 101       |

1.  output_agent.csv

output_agent.csv outputs the shortest time-space path of two days (d=1,2),
respectively, after iteration, as shown in **Table 9**.

LR_iteration: Lagrangian algorithm iteration number;

Stepsize: iteration step size;

path_node_seq: node sequence of vehicle travel path;

path_time_seq: the time when the vehicle reaches each node in the path;

Upper bound: the calculation result of upper bound

1.  output_paxprofitLog.csv

As shown in **Table 10**, output_paxprofitLog.csv outputs the passenger profit
corresponding to each iteration.

1.  output_solution.csv

As shown in **Table 11**, output_solution.csv outputs the number of nodes,
links, passengers, vehicles contained in case1 and CPU running time. It also
includes the cost corresponding to the shortest path per day (d=1,2). For
example, the cost of the shortest path for the first day(d=1) is 17, and the
cost of the shortest path for the second day(d=2) is 19. their gap value and
relative gap also are outputted.

**Table 9 output_agent.csv**

| LR_iteration | Stepsize | path_node_seq                         | path_time_seq                               |
|--------------|----------|---------------------------------------|---------------------------------------------|
| 0            | 1.000000 | 11;4;2;1;12                           | 1;2;4;6;7                                   |
| Upper bound  |          | 11;4;2;7;2;5;9;5;6;8;6;3;10;3;1;12;   | 1;2;4;5;6;7;8;9;10;11;12;13;14;15;17;18;    |
| LR_iteration | Stepsize | path_node_seq                         | path_time_seq                               |
| 0            | 1.000000 | 11;4;2;1;12                           | 1;2;4;6;7                                   |
| Upper bound  |          | 11;4;2;7;2;5;9;5;6;8;6;4;3;10;3;1;12; | 1;2;4;5;6;7;8;9;10;11;12;13;15;16;17;19;20; |

**Table 10 output_paxprofitLog.csv**

| iteration | Stepsize | p1's profit | p2's profit | p3's profit | p4's profit | p5's profit | p6's profit | p7's profit | p8's profit | p9's profit | p10's profit | LowerBoundCost |
|-----------|----------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|--------------|----------------|
| 0         | 1        | 6           | 6           | 0           | 0           | 0           | 0           | 0           | 0           | 0           | 0            | 18             |

**Table 11 output_solution.csv**

| number of nodes=      | 6         |            |          |                   |     |              |
|-----------------------|-----------|------------|----------|-------------------|-----|--------------|
| number of links=      | 13        |            |          |                   |     |              |
| number of passengers= | 2         |            |          |                   |     |              |
| number of vehicles=   | 1         |            |          |                   |     |              |
| 18.000000             | 17.000000 | \-1.000000 | \-5.882% | CPU Running Time= | 307 | milliseconds |
| number of nodes=      | 6         |            |          |                   |     |              |
| number of links=      | 13        |            |          |                   |     |              |
| number of passengers= | 2         |            |          |                   |     |              |
| number of vehicles=   | 1         |            |          |                   |     |              |
| 18.000000             | 19.000000 | 1.000000   | 5.263%   | CPU Running Time= | 238 | milliseconds |

1.  Debug.txt

Debug.txt is shown in **Figure 9**. The file outputs the following information
in turn:

a. the number of virtual nodes, the number of virtual links, the number of
passengers and the number of vehicles;

b. virtual links and corresponding link_type,

c. The original and destination of the vehicle, as well as the updated
information in the LR and DP calculations.

![**文本 描述已自动生成**](media/fb342769c66d36b9cf2f8469e7a11181.png)

**Figure 9 Debug.txt**

# 6 Tests

The test data set is stored in the dataset folder, and case1-4 correspond to:

**Scenario 1** The travel time in the historical data just meets the time
window, and no passenger waiting or vehicle waiting behaviour occurs;

**Scenario 2** There are changes in travel time of specific days in the
historical data, causing passengers to wait;

**Scenario 3** There are changes in travel time of specific days in the
historical data, causing vehicles to wait;

**Scenario 4** There are changes in the travel time of specific days in the
historical data, which makes it impossible to meet the time window of some
passengers, resulting in passengers not being served.

The physical network remains the same. Input_node.csv is the same as the example
of 5.1 section. Change the two columns of travel_time_1 and travel_time_2 in the
input_link_csv file according to **Table 12**, and the input_agent.csv file
according to **Table 13**. The final results of the four scenarios are shown in
**Table 14**.

**Table 12 Travel time of four scenarios**

| link  | Scenario 1 | Scenario 2 | Scenario 3 | Scenario 4 |      |       |      |       |
|-------|------------|------------|------------|------------|------|-------|------|-------|
|       | Day1       | Day2       | Day1       | Day2       | Day1 | Day2  | Day1 | Day2  |
| (1,2) | 2          | 2          | 2          | 2          | 2    | 2     | 2    | 2     |
| (1,3) | 2          | 2          | 2          | 2          | 2    | 2     | 2    | 2     |
| (1,5) | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (2,1) | 2          | 2          | 2          | 2          | 2    | 2     | 2    | 2     |
| (2,4) | 2          | 2          | 2          | 2          | 2    | 2     | 2    | 2     |
| (2,5) | 1          | 1          | 1          | **2**      | 1    | 1     | 1    | **4** |
| (3,1) | 2          | 2          | 2          | 2          | 2    | 2     | 2    | 2     |
| (3,4) | 2          | 2          | 2          | 2          | 2    | 2     | 2    | 2     |
| (4,2) | 2          | 2          | 2          | 2          | 2    | **1** | 2    | 2     |
| (4,3) | 2          | 2          | 2          | 2          | 2    | 2     | 2    | 2     |
| (5,6) | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (6,3) | 1          | **6**      | 1          | 1          | 1    | 1     | 1    | 1     |
| (6,4) | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (2,)  | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (,2)  | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (6,)  | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (,6)  | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (5,)  | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (,5)  | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (3,)  | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (,3)  | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (,4)  | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |
| (1,)  | 1          | 1          | 1          | 1          | 1    | 1     | 1    | 1     |

>   **Table 13 Passenger and vehicle information of four scenarios**

|                           | Scenario 1 | Scenario 2 | Scenario 3 | Scenario 4 |         |         |         |         |
|---------------------------|------------|------------|------------|------------|---------|---------|---------|---------|
|                           | Day1       | Day2       | Day1       | Day2       | Day1    | Day2    | Day1    | Day2    |
| the number of passengers  | 2          | 2          | 2          | 2          | 2       | 2       | 2       | 2       |
| the number of vehicles    | 1          | 1          | 1          | 1          | 1       | 1       | 1       | 1       |
| original of               | Node 2     | Node 2     | Node 2     | Node 2     | Node 2  | Node 2  | Node 2  | Node 2  |
| destination of            | Node 6     | Node 6     | Node 6     | Node 6     | Node 6  | Node 6  | Node 6  | Node 6  |
| original of               | Node 5     | Node 5     | Node 5     | Node 5     | Node 5  | Node 5  | Node 5  | Node 5  |
| destination of            | Node 3     | Node 3     | Node 3     | Node 3     | Node 3  | Node 3  | Node 3  | Node 3  |
| original of               | Node 4     | Node 4     | Node 4     | Node 4     | Node 4  | Node 4  | Node 4  | Node 4  |
| destination of            | Node 1     | Node 1     | Node 1     | Node 1     | Node 1  | Node 1  | Node 1  | Node 1  |
| departure time window of  | [5,7]      | [5,7]      | [5,7]      | [5,7]      | [5,7]   | [5,7]   | [5,7]   | [5,7]   |
| arrival time window of    | [9,12]     | [9,12]     | [9,12]     | [9,12]     | [9,12]  | [9,12]  | [9,13]  | [9,13]  |
| departure time window of  | [8,10]     | [8,10]     | [8,10]     | [8,10]     | [8,10]  | [8,10]  | [8,10]  | [8,10]  |
| arrival time window of    | [14,19]    | [14,19]    | [14,19]    | [14,19]    | [14,19] | [14,19] | [14,19] | [14,19] |
| departure time window of  | [1,1]      | [1,1]      | [1,1]      | [1,1]      | [1,1]   | [1,1]   | [1,1]   | [1,1]   |
| arrival time window of    | [1,30]     | [1,30]     | [1,30]     | [1,30]     | [1,30]  | [1,30]  | [1,30]  | [1,30]  |

>   **Table 14 Results of four scenarios**

| Iteration                                                          | Path_node_seq                        | Lowerbound | Upperbound | Gap(%) |
|--------------------------------------------------------------------|--------------------------------------|------------|------------|--------|
| Scenario 1 No passenger waiting or vehicle waiting behavior occurs |                                      |            |            |        |
| 1                                                                  | 11,4,2,1,12                          | 6          | 19         | 68.42  |
| 2                                                                  | 11,4,2,7,2,5,9,5,6,8,6,4,3,10,3,1,12 | 19         | 19         | 0.00   |
| Scenario 2 Passenger waiting                                       |                                      |            |            |        |
| 1                                                                  | 11,4,2,1,12                          | 6          | 17.65      | 66.01  |
| 2                                                                  | 11,4,2,7,2,5,9,5,6,8,6,3,10,3,1,12   | 17.65      | 17.65      | 0.00   |
| Scenario 3 Vehicle waiting                                         |                                      |            |            |        |
| 1                                                                  | 11,4,2,1,12                          | 6          | 16.75      | 64.18  |
| 2                                                                  | 11,4,2,7,2,5,9,5,6,8,6,3,10,3,1,12   | 16.75      | 16.75      | 0.00   |
| Scenario 4 Passenger not being served                              |                                      |            |            |        |
| 1                                                                  | 11,4,2,1,12                          | 6          | 19.6       | 68.34  |
| 2                                                                  | 11,4,2,7,2,1,5,9,5,6,8,6,3,10,3,1,12 | 19.6       | 19.6       | 0.00   |

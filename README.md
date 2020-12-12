# Stochastic-VRPPDTW

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


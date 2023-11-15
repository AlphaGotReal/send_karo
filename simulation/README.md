# Simulation package for IGVC 2019.

### Launching the simulation

Clone the package into the ```src``` folder of a workspace along with ```robot_description```

And then launch simulation by using command:

```
roslaunch simulation igvc_sim.launch
```

### Launching with arguments

Use the syntax:

```
roslaunch simulation igvc_sim.launch {arg_name_1}:={arg_value_1} .... {arg_name_n}:={arg_value_n}
```
*Replace {arg_name_x} by argument name and {arg_value_x} by argument value*

Example:

```
roslaunch simulation igvc_sim.launch gui:="false"
```

#### Arguments available

- **include_potholes** *(default:"true")* : Includes potholes in the world.
- **gui** *(default:"true")* : Toggles the Gazebo GUI on (*true*) or off (*false*)
- **paused** *(default:"false")* : Launches the simulation in paused state

Extra arguments available in launch file:
- **use_sim_time** *(default:"true")* : If enabled, all the communication happens with respect to the simulation clock.
- **verbose** *(default:"true")* : Launches Gazebo in verbose mode.
- **debug** *(default:"false")* : Enable Gazebo debugging.
- **recording** *(default:"false")* : Enable this to record simulation logs.

# C++ Implementation of File Monitor 

## Build Inotify

Go into the inotify folder and run make to build the library. This library provides a wrapper in c++ so we can monitor files for changes

## Build PTPMonitor

Create a new build folder to compile the project.

```
mkdir build
cd build
```

Once in the build folder run cmake to compile the project. Make sure you have built the inotify library or it will not work. Specify that you want to use the "CMakeLists.txt" folder in the previous directory with two periods ".."

```
cmake ..
```

## Run PTPMonitor

Once the executable is build it should appear in the build folder as "ptp_monitor"
To run the monitor you can simple run 

```
./ptp_monitor
```

Which will monitor the file for 10 seconds by default and print everything to the screen

To run the monitor for a specified amount of time you can speicfy the command line options below:

```
-days or -d:   Number of days
-hours or -h:  Number of hours
-minutes or -m: number of minutes
-seconds or -s: number of seconds
-run_type or -r: print everything to the screen "live" or write to output file "file"

```

It will default to running live, and the output folder will be named "ptp_monitor.txt" if you choose to write everything to a file. 

When running the monitor live the values are converted to microseconds (Clock Offset) and milliseconds (Path Delay) with the units being printed alongside the values, but there is not conversion when writing to a file and everything is left in seconds. 

Run for 3 hours live example:

```
./ptp_monitor -hours 3
```

Run for 3 hours file example

```
./ptp_monitor -hours 3 -run_type file
```
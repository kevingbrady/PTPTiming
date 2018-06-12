# Run PTP Monitor 

To run just specify the total number of seconds your want to run the monitor, and whether you want the output to print to the screen or to a file
It defaults to printing to the screen so put "file" on the command line if you want to save the output

Run live for 1 hour
```
python3 ptp_monitor.py 3600
```

Write to file for 1 hour
```
python3 ptp_monitor 3600 file
```
# PTPd Daemon

To download ptp daemon from linuc command line run:

```
sudo apt-get install ptpd
```

Then create a ptp directory in /etc where you can keep all the necessary config and output files for the service

```
sudo mkdir /etc/ptp
```

From here change the directory owner to the current user and move the ptpd.conf file into the directory you just created

```
sudo chown <user_name> /etc/ptp
mv ptpd.conf /etc/ptp
```

Now create a text file called "status.txt" that the service will log information to. This is the file that the PTPMonitor will keep track of to measure clock offset and path delay information

```
cd /etc/ptp
touch status.txt
```

Now go into the /etc/default folder to enable the service to run

```
cd /etc/default
sudo nano ptpd
```

Make sure the ptpd file in this folder looks like this:

```
# /etc/default/ptpd

# Set to "yes" to actually start ptpd automatically
START_DAEMON=yes

# Add command line options for ptpd
PTPD_OPTS="-c /etc/ptp/ptpd.conf"

```

Then you can start, stop, and restart the service as you would any other on the machine

```
sudo service ptpd start
sudo service ptpd stop
sudo service ptpd restart
```

Once you have the service up and running you can check the /etc/ptp/status.txt folder to see if it is updating values. If so you can run the monitor to see the current offset and path delay information.

If you run the PTPMonitor and it is printing out all zeros then the firewall is probably blocking the service from using the incoming packets from the grandmaster clock. You have to open ports 319 and 320 to get around this

You can run the PTPMonitor using Python or C++, but the C++ implementation provides a better estimate of the cpu usage of the PTPd service
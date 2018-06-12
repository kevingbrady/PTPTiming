import os
import pyinotify
import re
import time
import sys
from python.check import is_number

class PTP_Status_Watcher(pyinotify.ProcessEvent):

    start = 0
    statusfile = ''
    offset = 0
    mean = 0
    std = 0
    last_modify = 0
    cpu_temperature = 0
    idle_cpuload = 0
    total_cpuload = 0
    cpu_percentage = 0.0

    def __init__(self, f, t, test_type):

        self.statusfile = f
        self.start = time.time()
        self.test_duration = float(t)
        self.run_type = test_type
        if(self.run_type == 'file'):

            self.outputFile = open('ptp_monitor.txt', 'w+')
            self.outputString = ''

    def set_cpu_temperature(self):

        f = open("/sys/class/thermal/thermal_zone0/temp", 'r').readline()
        self.cpu_temperature = float(f)/1000

    def set_cpu_load(self):

        f = open("/proc/stat", 'r').readline()
        cpustring, input1, input2, input3, idle_cpuload, input4, input5, input6, input7, input8, input9 = f.split()
        input1, input2, input3, idle_cpuload, input4, input5, input6, input7, input8, input9 = int(input1), int(input2), int(input3), int(idle_cpuload), int(input4), int(input5), int(input6), int(input7), int(input8), int(input9)
        total_cpuload = input1 + input2 + input3 + input4 + input5 + input6 + input7 + input8 + input9 + idle_cpuload
        cpu_percentage = 1.0 - float((idle_cpuload - self.idle_cpuload)/(total_cpuload - self.total_cpuload))

        self.cpu_percentage = round(cpu_percentage * 100, 2)
        self.idle_cpuload = idle_cpuload
        self.total_cpuload = total_cpuload

    def process_IN_MODIFY(self, evt):

        if(time.time() - self.start > self.test_duration):

            if(self.run_type == 'file'):

                self.outputFile.write(self.outputString)
                self.outputFile.close()

            sys.exit()

        self.set_cpu_temperature()
        self.set_cpu_load()
        last_modify = float(os.stat(self.statusfile).st_mtime)

        f = open(self.statusfile, 'r')

        for line in f:
            
            if(line.startswith('Offset from Master')):

                try:

                    self.offset, self.mean, self.std = line.split(',', 3)
                    self.offset = float(re.search(r'\d+\.\d+', self.offset).group())
                    self.mean = float(re.search(r'\d+\.\d+', self.mean).group())
                    self.std = float(re.search(r'\d+\.\d+', self.std).group())

                    # subtract clock offset from file timestamp to get reference value from GM Clock
                    last_modify -= self.offset

                    # Convert offset, mean, standard deviation to microseconds for live viewing
                    if(self.run_type == 'live'):

                        self.offset *= 1000000
                        self.mean *= 1000000
                        self.std *= 1000000

                except ValueError:

                    pass

                
                if(last_modify != self.last_modify):

                    if(self.run_type == 'file'):

                        self.outputString += str(last_modify) + ' | ' + str(self.offset) + ' | ' + str(self.mean) +  ' | ' +  str(self.std) + ' | ' +  str(self.cpu_temperature) +  ' | '+ str(self.cpu_percentage) + '\n'

                        if(int(time.time() - self.start) % 300 == 0):

                            self.outputFile.write(self.outputString)
                            self.outputString = ''

                    else:

                        # round values and add units for live viewing
                        print(last_modify, '|', round(self.offset, 2), 'us |', round(self.mean, 2), 'us |', round(self.std, 2), 'us |', round(self.cpu_temperature, 2), 'C |', self.cpu_percentage, '%')

        self.last_modify = last_modify

if __name__ == '__main__':

    p = '/etc/ptp/status.txt'
    run_time = 10
    run_type = 'live'

    if(len(sys.argv) in (2, 3)):

        if(len(sys.argv) == 2):

            if(is_number(sys.argv[1])):

                run_time = sys.argv[1]

            else:

                run_type = sys.argv[1]

        if(len(sys.argv) == 3):

            
            if(is_number(sys.argv[1])):

                run_time = sys.argv[1]

            else:

                run_type = sys.argv[1]

            
            if(is_number(sys.argv[2])):

                run_time = sys.argv[2]

            else:

                run_type = sys.argv[2]

    handler = PTP_Status_Watcher(p, run_time, run_type)
    wm = pyinotify.WatchManager()
    notifier = pyinotify.Notifier(wm, handler)
    wdd = wm.add_watch(p, pyinotify.ALL_EVENTS)
    notifier.loop()


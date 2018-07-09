import os
import time
import pyinotify

class PTP_Status_Watcher(pyinotify.ProcessEvent):

    statusfile = ''
    def __init__(self, f):

        self.statusfile = f

    def process_IN_ACCESS(self, event):
        print("ACCESS event:", event.pathname)

    def process_IN_ATTRIB(self, event):
        print("ATTRIB event:", event.pathname)

    def process_IN_CLOSE_NOWRITE(self, event):
        print("CLOSE_NOWRITE event:", event.pathname)

    def process_IN_CLOSE_WRITE(self, event):
        print("CLOSE_WRITE event:", event.pathname)

    def process_IN_CREATE(self, event):
        print("CREATE event:", event.pathname)

    def process_IN_DELETE(self, event):
        print("DELETE event:", event.pathname)

    def process_IN_MODIFY(self, event):
        print("MODIFY event:", event.pathname)

    def process_IN_OPEN(self, event):
        print("OPEN event:", event.pathname)

if __name__ == '__main__':

    p = '/etc/ptp/status.txt'
    handler = PTP_Status_Watcher(p)
    wm = pyinotify.WatchManager()
    notifier = pyinotify.Notifier(wm, handler)
    wdd = wm.add_watch(p, pyinotify.ALL_EVENTS)
    notifier.loop()

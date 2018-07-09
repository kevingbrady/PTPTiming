import os

ip_addr = '129.6.59.132'
os.system('sudo scp ptp_monitor.txt kgb@' + ip_addr + ':/home/kgb/Shared/PyCharmProjects/PTPTiming/data')

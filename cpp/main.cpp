#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <thread>
#include <ctype.h>
#include <string>

#include "StatusLogWatcher.h"
#include "InotifyManager.h"

using namespace std;

int main(int argc, char *argv[]){

    string filePath = "/etc/ptp/status.txt";
    InotifyManager m;

	int days, hours, minutes, seconds;
	days = hours = minutes = seconds = 0;
    std::string run_type = "live";

	for (int i = 1; i < argc; i++) {

			if(std::string(argv[i]) == "-days" || std::string(argv[i]) == "-d") {

				days = std::atoi(argv[i + 1]);

			} else if(std::string(argv[i]) == "-hours" || std::string(argv[i]) == "-h") {

				hours = std::atoi(argv[i + 1]);

	        } else if(std::string(argv[i]) == "-minutes" || std::string(argv[i]) == "-m") {

				minutes = std::atoi(argv[i + 1]);

			} else if(std::string(argv[i]) == "-seconds" || std::string(argv[i]) == "-s") {

				seconds = std::atoi(argv[i + 1]);

			} else if (std::string(argv[i]) == "-run_type" || std::string(argv[i]) == "-r"){
			
				run_type = std::string(argv[i + 1]);

			}
	}

    int test_duration = (days * 86400) + (hours *  3600) + (minutes * 60) + seconds;

    if(test_duration == 0){
	
		test_duration = 10;

	}    

	std::cout << "Days: " << days << " Hours: " << hours << " Minutes: " << minutes << " Seconds: " << seconds << " Total Seconds: " << test_duration << " Run Type: " << run_type << std::endl;

    StatusLogWatcher ptpWatcher(filePath, test_duration, run_type);
    InotifyWatch  *w = m.addWatch(filePath, IN_MODIFY);

    w->addEventHandler(ptpWatcher);
    thread t = m.startWatching();
    t.join();

    w = nullptr;

    return 0;
}

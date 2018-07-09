#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <thread>
#include <ctype.h>
#include <string>

#include "StatusLogWatcher.h"
#include "Parameters.h"

int main(int argc, char *argv[]){

    auto params = std::make_shared<Parameters>();

    params->filePath = "/etc/ptp/status.txt";

	for (int i = 1; i < argc; i++) {

			if(std::string(argv[i]) == "-days" || std::string(argv[i]) == "-d") {

				params->days = std::atoi(argv[i + 1]);

			} else if(std::string(argv[i]) == "-hours" || std::string(argv[i]) == "-h") {

				params->hours = std::atoi(argv[i + 1]);

	        } else if(std::string(argv[i]) == "-minutes" || std::string(argv[i]) == "-m") {

				params->minutes = std::atoi(argv[i + 1]);

			} else if(std::string(argv[i]) == "-seconds" || std::string(argv[i]) == "-s") {

				params->seconds = std::atoi(argv[i + 1]);

			} else if (std::string(argv[i]) == "-run_type" || std::string(argv[i]) == "-r"){
			
				params->run_type = std::string(argv[i + 1]);

			}
	}

    params->total = (params->days * 86400) + (params->hours *  3600) + (params->minutes * 60) + params->seconds;

    StatusLogWatcher ptpWatcher(params);
    if(ptpWatcher.isServiceRunning("ptpd")){

        std::cout << "PTP Daemon Running ..." << "\n" << "Starting Monitor ..." << "\n\n" << std::endl;
		ptpWatcher.setupWatcher();

	}

    return 0;
}

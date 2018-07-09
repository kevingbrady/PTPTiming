//
// Created by kgb on 6/1/18.
//

#ifndef PROJECT_PTPMONITOR_H
#define PROJECT_PTPMONITOR_H

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include "InotifyManager.h"

class StatusLogWatcher : public InotifyEventHandler {

public:

    StatusLogWatcher(std::string filePath, int test_duration, std::string run_type);
    ~StatusLogWatcher();
    virtual void setupWatcher();
    void get_cpu_temperature();
    void get_cpu_load();
    bool checkPTPActive();
	virtual void everyAnnounceMessage(double&);
    bool handle(InotifyEvent &e) override;

	double cpu_load_percentage, cpu_temperature;

private:

    InotifyManager m;
    InotifyWatch  *w;

    std::string filePath, run_type, output;
    int test_duration, count, max_cpu_load_count;
    double offset, offset_mean, offset_std, path_delay, path_delay_mean, path_delay_std;    //values from log file to collect
    double start, last_modify, GM, announce_message_count;   // timestamps of test start and last modification of file
    float  previous_idle_load_time, previous_active_load_time;
    std::ofstream  *outputFile = nullptr;

};


#endif //PROJECT_PTPMONITOR_H

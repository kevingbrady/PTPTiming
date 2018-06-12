//
// Created by kgb on 6/1/18.
//

#ifndef PROJECT_PTPMONITOR_H
#define PROJECT_PTPMONITOR_H

#include <stdio.h>
#include "InotifyManager.h"

class StatusLogWatcher : public InotifyEventHandler {

public:

    StatusLogWatcher(std::string filePath, int test_duration, std::string run_type);
    ~StatusLogWatcher();
    void get_cpu_temperature();
    void get_cpu_load();
    bool handle(InotifyEvent &e) override;

private:

    std::string filePath, run_type, output;
    int test_duration, count, max_cpu_load_count;
    double offset, offset_mean, offset_std, path_delay, path_delay_mean, path_delay_std;    //values from log file to collect
    double start, last_modify, GM;   // timestamps of test start and last modification of file
    float  previous_idle_load_time, previous_active_load_time, cpu_load_percentage, cpu_temperature;
    std::ofstream  *outputFile = nullptr;

};


#endif //PROJECT_PTPMONITOR_H

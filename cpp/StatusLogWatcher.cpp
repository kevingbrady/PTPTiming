//
// Created by kgb on 6/1/18.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <math.h>
#include "StatusLogWatcher.h"

struct stat attrib;

StatusLogWatcher::StatusLogWatcher(std::string filePath, int test_duration, std::string run_type) : filePath(filePath), test_duration(test_duration), run_type(run_type){


    this->start = 0;

    if(this->run_type == "file"){

        this->outputFile = new std::ofstream("ptp_monitor.txt");
        this->output = "";
    }
}

StatusLogWatcher::~StatusLogWatcher(void){

    if(this->run_type == "file"){

        (*(this->outputFile)) << this->output;
        (*(this->outputFile)).close();
        this->outputFile = nullptr;
    }

}
void StatusLogWatcher::get_cpu_temperature(){

    std::ifstream fin("/sys/class/thermal/thermal_zone0/temp");
    double cpu_temp;
    fin >> cpu_temp;
    fin.close();

    cpu_temp /= 1000;
    this->cpu_temperature =  static_cast<float>(cpu_temp);

}

void StatusLogWatcher::get_cpu_load(){

    std::ifstream cpufile("/proc/stat");
    std::string cpu;
    float cpu_load_percentage, active_load_time, idle_load_time, idle, io_wait, user, nice, system, irq, softirq, steal, guest, guest_nice;
    float diff_idle, diff_active, diff_total;

    cpufile >> cpu >> user >> nice >> system >> idle >> io_wait >> irq >> softirq >> steal >> guest >> guest_nice;
    cpufile.close();

    active_load_time = user + nice + system + irq + softirq + steal + guest + guest_nice;
    idle_load_time = idle + io_wait;

    diff_idle = static_cast<float>(idle_load_time - this->previous_idle_load_time);
    diff_active = static_cast<float>(active_load_time - this->previous_active_load_time);
    diff_total = diff_idle + diff_active;

    cpu_load_percentage = (1.0 - (diff_idle/diff_total));

    if(diff_total < 5){

        cpu_load_percentage = 0.0;

    } else {

        cpu_load_percentage *= 100.f;

    }
    this->previous_idle_load_time = idle_load_time;
    this->previous_active_load_time = active_load_time;

    this->cpu_load_percentage = cpu_load_percentage;

}

bool StatusLogWatcher::handle(InotifyEvent &e){

    stat(this->filePath.c_str(), &attrib);
    double last_modify = attrib.st_mtime + (attrib.st_mtim.tv_nsec * 1e-9);
    if(last_modify != this->last_modify){

            this->get_cpu_temperature();
            this->get_cpu_load();

            std::ifstream statusFile(this->filePath);

            if(this->start > 0){

                std::string line, offsetString, offsetMeanString, offsetSTDString, pathString, pathMeanString, pathSTDString;
                double number, offset, mean, std;

                std::ostringstream gmF;
                std::ostringstream offsetF;
                std::ostringstream offset_meanF;
                std::ostringstream offset_stdF;
                std::ostringstream cputempF;
                std::ostringstream cpuloadF;
                std::ostringstream path_delayF;
                std::ostringstream path_delay_meanF;
                std::ostringstream path_delay_stdF;

                while(std::getline(statusFile, line)){

                    if(line.find("Offset from Master") == 0){

                        offsetString = line.substr(line.find(":") + 2, 12);
                        offsetMeanString = line.substr(line.find("mean") + 6, 12);
                        offsetSTDString = line.substr(line.find("dev") + 5, 12);

                        this->offset = std::atof(offsetString.c_str());
                        this->offset_mean = std::atof(offsetMeanString.c_str());
                        this->offset_std = std::atof(offsetSTDString.c_str());
                        this->GM = last_modify - this->offset;

                    }

                    if(line.find("Mean Path Delay") == 0){

                        pathString = line.substr(line.find(":") + 2, 12);
                        pathMeanString = line.substr(line.find("mean") + 6, 12);
                        pathSTDString = line.substr(line.find("dev") + 5, 12);

                        this->path_delay = std::atof(pathString.c_str());
                        this->path_delay_mean = std::atof(pathMeanString.c_str());
                        this->path_delay_std = std::atof(pathSTDString.c_str());

                    }

                }

                if(this->run_type == "file"){

                    gmF << std::fixed << std::setprecision(6) << this->GM;
                    offsetF << std::fixed << std::setprecision(9) << this->offset;
                    offset_meanF << std::fixed << std::setprecision(9) << this->offset_mean;
                    offset_stdF << std::fixed << std::setprecision(9) << this->offset_std;
                    path_delayF << std::fixed << std::setprecision(9) << this->path_delay;
                    path_delay_meanF << std::fixed << std::setprecision(9) << this->path_delay_mean;
                    path_delay_stdF << std::fixed << std::setprecision(9) << this->path_delay_std;
                    cputempF << std::fixed << std::setprecision(2) << this->cpu_temperature;
                    cpuloadF << std::fixed << std::setprecision(2) << this->cpu_load_percentage;

                    this->output += gmF.str() + ", " + offsetF.str() + ", " +  offset_meanF.str() + ", " + offset_stdF.str() + ", " + path_delayF.str() + ", " + path_delay_meanF.str() + ", " + path_delay_stdF.str() + ", " + cputempF.str() + ", " + cpuloadF.str() + "\n";

                    if((int)(last_modify - start) % 300 == 0){

                            (*(this->outputFile)) << this->output;
                            this->outputFile->flush();
                            output.clear();
                    }

                } else {

                      gmF << std::fixed << std::setprecision(6) << (this->GM);
                      offsetF << std::fixed << std::setprecision(3) << (this->offset * 1e6);    //--------- covert to microseconds ------------------
                      offset_meanF << std::fixed << std::setprecision(3) << (this->offset_mean * 1e6);
                      offset_stdF << std::fixed << std::setprecision(3) << (this->offset_std * 1e6); // ---------------------------------------------
                      path_delayF << std::fixed << std::setprecision(6) << (this->path_delay * 1000); // -------- convert to milliseconds -----------
                      path_delay_meanF << std::fixed << std::setprecision(6) << (this->path_delay_mean * 1000);
                      path_delay_stdF << std::fixed << std::setprecision(6) << (this->path_delay_std * 1000); //--------------------------------------
                      cputempF << std::fixed << std::setprecision(2) << this->cpu_temperature;
                      cpuloadF << std::fixed << std::setprecision(2) << this->cpu_load_percentage;

                      std::cout << gmF.str() + ", " + offsetF.str() + " us , " + offset_meanF.str() + " us , " + offset_stdF.str() + " us , " + path_delayF.str() + " ms, " + path_delay_meanF.str() + " ms, " + path_delay_stdF.str() + " ms, " + cputempF.str() + " C, " + cpuloadF.str() + "% \n";

                    }

                if(last_modify > (this->start + test_duration)){               //END TEST

                   return true;

                }

            } else {

                  this->start = last_modify;

              }

            statusFile.close();
    }

    this->last_modify = last_modify;

    return false;

}


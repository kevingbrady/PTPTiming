#include "PTPSensor.h"

PTPSensor::PTPSensor(std::shared_ptr<Parameters> params) : StatusLogWatcher(params) {

		this->sensor_value = 0;
		this->cpu_temperature = 0;
		this->cpu_load_percentage = 0;

		if(this->isServiceRunning("ptpd")){

            std::cout << "Starting Sensor ..." << "\n\n" << std::endl;;
            this->setupWatcher();

		}
}

PTPSensor::~PTPSensor(){

}

void PTPSensor::everyAnnounceMessage(double& last_modify){

		std::ostringstream timestamp, sensor, temp, cpu_usage;

		//Put Code to Read From Sensor Here

	    this->sensor_value = (rand() % 100) + 1;

		timestamp << std::fixed << std::setprecision(6) << last_modify;
		sensor << std::fixed << std::setprecision(3) << this->sensor_value;
		temp << std::fixed << std::setprecision(3) << this->cpu_temperature;
		cpu_usage << std::fixed << std::setprecision(3) << this->cpu_load_percentage;


		std::cout <<  timestamp.str() << "  " << sensor.str() <<  "  " << temp.str() << " " << cpu_usage.str() << std::endl;


}

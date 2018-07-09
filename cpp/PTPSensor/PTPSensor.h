//
// Created by kgb on 6/1/18.
//

#ifndef PROJECT_PTPSENSOR_H
#define PROJECT_PTPSENSOR_H

#include <cstdlib>
#include "StatusLogWatcher.h"

class PTPSensor : public StatusLogWatcher {

public:

	PTPSensor(int);
	~PTPSensor();
	void everyAnnounceMessage(double&) override;
    
private:

	std::string filePath;
    double sensor_value;

};


#endif //PROJECT_PTPSENSOR_H

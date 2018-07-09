#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>

class Parameters {

	public:

		Parameters(){};
		~Parameters(){};

		int days = 0;
		int hours = 0;
		int minutes = 0;
		int seconds = 0;
		int total = 10;

		std::string run_type = "live";
		std::string filePath;

};

#endif //PARAMETERS_H


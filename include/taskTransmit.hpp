#ifndef TASK_TRANSMIT
#define TASK_TRANSMIT

#include "RtosTask.hpp"

class TransmitTask: public RtosTask {
private:
	void setup() override;
	void loop() override;
};

#endif
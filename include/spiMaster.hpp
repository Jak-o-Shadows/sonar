#ifndef SpiMaster
#define SpiMaster

#include <stdint.h>

class SpiMaster{
    public:
    void setup();
    void transfer_bytes(uint16_t *data, uint16_t numHalfWords);

    private:
}


#endif
#include "payload_v2.h"

// define the payload's static counts
size_t Payload::ctor_count_ = 0;
size_t Payload::dtor_count_ = 0;

// switch console logging on/off
bool Payload::do_logging_ = false;


uint8 TYPE_INT = 0
uint8 TYPE_FLOAT = 1
uint8 STATUS_OK = 0
uint8 STATUS_NOT_FOUND = 1
uint8 STATUS_ERROR = 2

uint64 timestamp
char[16] param_name
int32 int_value
float32 float_value
uint8 param_type
uint8 status

uint8 ORB_QUEUE_LENGTH = 8

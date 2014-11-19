#define SYS_LOG_TAG "MAIN"
#include "syslog.h"

void main() {
    LOGD("hello world\n");
    LOGE("This is an error message!\n");
}

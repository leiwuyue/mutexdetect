# mutexdetect
detect block or mutex block

在检测卡住位置或者mutex 锁持锁位置，调用
#include <scheduler/MyTestRef.h>

sp<MyTestRef> spRefTest = new MyTestRef(__FILE__,__FUNCTION__,__LINE__);


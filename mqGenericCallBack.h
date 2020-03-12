#ifndef __MQGENERICCALLBACK_H__
#define __MQGENERICCALLBACK_H__

#include "MQTTAsync.h"
class mqGenericCallBack{
public:
    virtual void messageArrived(char* topicName, int topicLen, MQTTAsync_message* message) = 0;
    virtual void connectLost(char* cause) = 0;
    virtual void connectSuccess(MQTTAsync_successData* response) = 0;
    virtual void connectFail(MQTTAsync_failureData* response) = 0;
    virtual void subscribeOk(MQTTAsync_successData* response) = 0;
    virtual void subscribeFail() = 0;
};

#endif

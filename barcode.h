#ifndef _BARCODE_H_
#define _BARCODE_H_

#include "mqGenericCallBack.h"
#include <string>
using namespace std;

class barcode : public mqGenericCallBack
{
public:
    barcode();
	void onGetStr(const string& str);
    void messageArrived(char* topicName, int topicLen, MQTTAsync_message* message);
    void connectLost(char* cause);
    void connectSuccess(MQTTAsync_successData* response);
    void connectFail(MQTTAsync_failureData* response);
    void subscribeOk(MQTTAsync_successData* response);
    void subscribeFail();
	string generatePacket(const string& id);
    string getuuid();
private:
    string preStr;
};

#endif

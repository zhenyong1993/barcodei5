#ifndef _LOCAL_MQ_CLIENT_H
#define _LOCAL_MQ_CLIENT_H

#include "MQTTAsync.h"
#include <string>
#include "MQTTClient.h"
#include <iostream>
#include <vector>
#include <map>
#include "mqGenericCallBack.h"
using namespace std;

typedef enum
{
	LOCALMQ_DISCONNECTED=0,
	LOCALMQ_CONNECTING,
	LOCALMQ_CONNECTED
}LOCALMQ_CONNECT_STATUS;

//extern "C" int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);

typedef struct _subTag
{
	string subTopic;
	int subQos;
}subTag;

typedef struct _pubTag
{
	string payLoad;
	string pubTopic;
}pubTag;

class mqClient
{
public:
    //setContext should be called before getInstance
    static bool setContext(void* context);
	static mqClient& getInstance();
	int connect();
	static void connlost(void *context, char *cause);
	static int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message);
    static void deliverComplete(void* context, MQTTAsync_token token);
	static void onConnectFailure(void* context, MQTTAsync_failureData* response);
	static void onConnected(void* context, MQTTAsync_successData* response);
	static void onSubscribeOk(void* context, MQTTAsync_successData* response);
	static void onSubscribeFailure(void* context, MQTTAsync_failureData* response);
	static void onUnsubscribeOk(void* context, MQTTAsync_successData* response);
	static void onUnsubscribeFailure(void* context, MQTTAsync_failureData* response);
	static void publishOk(void *context, MQTTAsync_successData *respons);
	static void publishFail(void* context, MQTTAsync_failureData* response);
	int publish(const string& _topic,const string& _payload ,int _qos=1);
	int subscribe(const string &_topic, int _qos);
    int unsubscribe(const string &_topic);
	bool checkStatus();
    static mqGenericCallBack* contextM;
private:
	mqClient();
private:
	MQTTAsync client;
	string address;
	string clientId;
	int Qos;
	int TIMEOUT;
	static LOCALMQ_CONNECT_STATUS status;
	static bool disconnected;
	MQTTAsync_connectOptions conn_opts;	
	map<int,subTag> subTopicList;
	map<int,pubTag> pubTopicList;
};


#endif


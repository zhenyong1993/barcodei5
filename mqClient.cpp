#include "mqClient.h"

LOCALMQ_CONNECT_STATUS mqClient::status = LOCALMQ_DISCONNECTED;
bool mqClient::disconnected = false;
mqGenericCallBack* mqClient::contextM = nullptr;

mqClient::mqClient()
    :conn_opts(MQTTAsync_connectOptions_initializer),
    subTopicList(),
    pubTopicList()
{
	address = "tcp://127.0.0.1:1883";
	clientId = "agentDriver";
	Qos = 1;
	TIMEOUT = 1;
	MQTTAsync_create(&client, address.c_str(), clientId.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
	MQTTAsync_setCallbacks(client, contextM, connlost, msgarrvd, deliverComplete);
    conn_opts.struct_version = 2;
	conn_opts.connectTimeout = 5;
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnected;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = contextM;
    conn_opts.automaticReconnect = 1;
    conn_opts.maxRetryInterval = 5;
}

mqClient& mqClient::getInstance()
{
	static mqClient instance;
	return instance;
}

bool mqClient::setContext(void* context)
{
    if(nullptr == contextM)
    {
        contextM = (mqGenericCallBack*)context;
        return true;
    }
    else
        return false;
}

int mqClient::connect()
{
	int rc = 0;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		cout<<(("mqttClient Failed to start connect, return code\n" + to_string(rc)))<<endl;
		status = LOCALMQ_DISCONNECTED;
	}		
	else
	{
		status = LOCALMQ_CONNECTING;
	}
    cout << "connect rc is " << rc << endl;
    return rc;
}

void mqClient::connlost(void *context, char *cause)
{
    ((mqGenericCallBack*)context)->connectLost(cause);
	status = LOCALMQ_DISCONNECTED;

	disconnected = true;
}	
void mqClient::deliverComplete(void* context, MQTTAsync_token token)
{

}


void mqClient::onConnectFailure(void* context, MQTTAsync_failureData* response)
{
	cout << (("mqttClient Connect failed, rc " + to_string(response ? response->code : 0))) << endl;;
	status = LOCALMQ_DISCONNECTED;
    ((mqGenericCallBack*)context)->connectFail(response);
}

void mqClient::onConnected(void* context, MQTTAsync_successData* response)
{
	status = LOCALMQ_CONNECTED;
    ((mqGenericCallBack*)context)->connectSuccess(response);
}



void mqClient::publishOk(void *context, MQTTAsync_successData *respons)
{
	if(respons)
	{
		cout<<"token "<<respons->token<<" pulish succedd"<<endl;	
	}
}

void mqClient::publishFail(void* context, MQTTAsync_failureData* response)
{
	if(response)
	{
		cout<<"token "<<response->token<<" pulish fail"<<endl;		
	}
}


int mqClient::publish(const string& _topic,const string& _payload,int _qos )
{
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	int rc;
	
	pubmsg.payload = (char*)_payload.c_str();
	pubmsg.payloadlen = _payload.length();
	pubmsg.qos = _qos;
	pubmsg.retained = 0;

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = publishOk;
	opts.onFailure = publishFail;

	if ((rc = MQTTAsync_sendMessage(client, _topic.c_str(), &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
	{
		cout<<(("mqttClient Failed to start sendMessage, return code " + to_string(rc)))<<endl;
	}	
	else
	{
		cout<<"publish token is "<<opts.token<<endl;
	}
	if (MQTTASYNC_DISCONNECTED == rc)
	{
		status = LOCALMQ_DISCONNECTED;
	}
	return rc;
}

bool mqClient::checkStatus()
{
	bool ret = false;
	if(status==LOCALMQ_DISCONNECTED)
	{
		connect();
	}
	else if(LOCALMQ_CONNECTED==status)
	{
		ret = true;
	}
	return ret;
}

int mqClient::msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
    ((mqGenericCallBack*)context)->messageArrived(topicName, topicLen, message);
    //TODO: free message
    /*
	int i;
	char* payloadptr;
	if (disconnected)
	{
		printf("Message arrived\n");
		printf("     topic: %s\n", topicName);
		printf("   message: ");
	}
	disconnected = false;

	payloadptr = (char*)message->payload;
    */
	/*
	for (i = 0; i<message->payloadlen; i++)
	{
		putchar(*payloadptr++);
	}
	putchar('\n');
	*/
//	MQTTAsync_freeMessage(&message);
//	MQTTAsync_free(topicName);
	return 1;
}


void mqClient::onSubscribeOk(void* context, MQTTAsync_successData* response)
{
    ((mqGenericCallBack*)context)->subscribeOk(response);
}

void mqClient::onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Subscribe failed, rc %d\n", response ? response->code : 0);
}

void mqClient::onUnsubscribeOk(void* context, MQTTAsync_successData* response)
{
    cout << "unsubscribe ok" << endl;
}

void mqClient::onUnsubscribeFailure(void* context, MQTTAsync_failureData* response)
{
    cout << "unsubscribe fail" << endl;
}

int mqClient::subscribe(const string &_topic,int _qos)
{
	MQTTAsync_responseOptions sub_opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	int rc;

	sub_opts.onSuccess = onSubscribeOk;
	sub_opts.onFailure = onSubscribeFailure;
	sub_opts.context = contextM;
	
	if ((rc = MQTTAsync_subscribe(client, _topic.c_str(), _qos, &sub_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start subscribe, return code %d\n", rc);
	}
	else
	{
		subTag tag;
		tag.subQos = _qos;
		tag.subTopic = _topic;
		subTopicList.insert(pair<int,subTag>(sub_opts.token,tag));
	}
	return rc;
}

int mqClient::unsubscribe(const string &_topic)
{
	MQTTAsync_responseOptions sub_opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	int rc;

	sub_opts.onSuccess = onUnsubscribeOk;
	sub_opts.onFailure = onUnsubscribeFailure;
	sub_opts.context = contextM;
	
	if ((rc = MQTTAsync_unsubscribe(client, _topic.c_str(), &sub_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start unsubscribe, return code %d\n", rc);
	}
	else
	{
		subTag tag;
		tag.subTopic = _topic;
		subTopicList.insert(pair<int,subTag>(sub_opts.token,tag));
	}
	return rc;
}

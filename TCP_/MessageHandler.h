#pragma once

#include <thread>
#include "MessageQueue.h"

class MessageHandler {
private:
	thread looper;
	MessageQueue mq;
	bool handlerRunning;

public:
	MessageHandler() {
		handlerRunning = false;
	}
	~MessageHandler() {}
	bool getHandlerRunning(){ return handlerRunning; }
	void start();
	void stop();
	void recved(string message);
	void loop();
};
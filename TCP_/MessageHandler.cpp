#include "MessageHandler.h"
#include <stdio.h>

void MessageHandler::start() {
	handlerRunning = true;
	looper = std::thread(&MessageHandler::loop, this);
}

void MessageHandler::stop() {
	handlerRunning = false;
	if (looper.joinable()) {
		looper.join();
	}
	mq.destroy();
}

void MessageHandler::recved(string message) {
	mq.enqueue(message);
}

void MessageHandler::loop() {
	while (handlerRunning) {
		string message = mq.dequeue();

		printf("Recved : %s\n",message.c_str());
		if (handlerRunning == false)
			break;
	}
}
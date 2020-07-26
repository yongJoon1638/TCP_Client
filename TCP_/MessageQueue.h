#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;

class MessageQueue {
private:
	queue<string> recved;
	mutex m;
	condition_variable cv;
	bool running;
public:
	MessageQueue() {
		running = true;
	}
	~MessageQueue() {}

	void enqueue(string val) {
		lock_guard<mutex> lock(m);
		recved.push(val);
		cv.notify_one();
	}

	string dequeue() {
		unique_lock<mutex> lock(m);
		while (running && recved.empty()) {
			cv.wait(lock);
		}
		string val;
		val = recved.front();
		recved.pop();
		return val;
	}

	void destroy() {
		running = false;
		cv.notify_one();
	}
};
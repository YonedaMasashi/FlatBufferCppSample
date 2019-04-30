#pragma once

#include <iostream>
#include <chrono>

using namespace std;

class TimeMeasure {

public:
	TimeMeasure();

	virtual ~TimeMeasure();

public:

	void start();
	string end();

private:
	chrono::system_clock::time_point startTimePoint;

};
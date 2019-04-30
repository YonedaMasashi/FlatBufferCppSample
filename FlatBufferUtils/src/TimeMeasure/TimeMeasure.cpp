#include "TimeMeasure/TimeMeasure.h"

TimeMeasure::TimeMeasure() {}

TimeMeasure::~TimeMeasure() {}

void TimeMeasure::start() {
	startTimePoint = chrono::system_clock::now();
}

string TimeMeasure::end() {
	chrono::system_clock::time_point endTimePoint = chrono::system_clock::now();

	char chBuf[1024] = { 0 };
	sprintf_s(chBuf, "%lld", std::chrono::duration_cast<std::chrono::milliseconds>(endTimePoint - startTimePoint).count());

	return string(chBuf);
}
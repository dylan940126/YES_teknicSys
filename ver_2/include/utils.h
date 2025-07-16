#ifndef UTILS_H
#define UTILS_H

#include <thread>
#include <chrono>

// Cross-platform Sleep replacement
inline void SleepMs(unsigned int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

#endif

using namespace std;
bool CheckContinue();

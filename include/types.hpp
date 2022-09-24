#pragma once
#include <iostream>
#include <chrono>
#define TIME_IT(process) \
do  \
{ \
  auto begin = std::chrono::system_clock::now(); \
  process; \
  auto end = std::chrono::system_clock::now(); \
  printf("%s perf: %d [mks]\n", #process, std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count()); \
}while(0);
struct GPIOPin
{
    GPIOPin(): port(0), pin(0){}
    GPIOPin(char port, char pin)
    {
        this->port = port;
        this->pin = pin;
    }
    bool operator==(const GPIOPin& gpio) const
    {
        return ((GPIOPin::pin == gpio.pin) && (GPIOPin::port == gpio.port)) ? true: false;
    }
    bool operator!=(const GPIOPin& gpio) const
    {
        return ((GPIOPin::pin == gpio.pin) && (GPIOPin::port == gpio.port)) ? false: true;
    }
    char port; 
    char pin;
};

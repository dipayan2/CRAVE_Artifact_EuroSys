#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

#include "cpuclock.h"
#include "memclock.h"
#include "gpuclock.h"

int main(int argc, char** argv){

    CpuClock myCPU(0);
    std::cout<< "The CPU Utilization " << myCPU.GetUtilization() << std::endl;
    std::cout<< "The CPU Clock: " << myCPU.GetClock() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout<< "The CPU Utilization: " << myCPU.GetUtilization() << std::endl;
    myCPU.SetClock(1113600);            // Some funny business is happening here
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout<< "The CPU Utilization " << myCPU.GetUtilization() << std::endl;
    std::cout<< "The CPU Clock: " << myCPU.GetClock() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    myCPU.SetClock(1420800);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout<< "The CPU Utilization " << myCPU.GetUtilization() << std::endl;
    std::cout<< "The CPU Clock: " << myCPU.GetClock() << std::endl;

    myCPU.unSetDevice();
    // Test the memory system.


    MemClock myMem;

    std::cout<< "My Memorry utilization is : "<< myMem.GetUtilization() << std::endl;
    std::cout<< "The Memory Clock: " << myMem.GetClock() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout<< "My Memorry utilization is : "<< myMem.GetUtilization() << std::endl;
    myMem.SetClock(1062400000);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout<< "My Memorry utilization is : "<< myMem.GetUtilization() << std::endl;
    std::cout<< "The Memory Clock: " << myMem.GetClock() << std::endl;
    myMem.SetClock(1866000000);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout<< "My Memorry utilization is : "<< myMem.GetUtilization() << std::endl;
    std::cout<< "The Memory Clock: " << myMem.GetClock() << std::endl;
    myMem.unSetDevice();

    // Test the GPU system.

    GpuClock myGpu;

    std::cout<< "My GPU utilization is : "<< myGpu.GetUtilization() << std::endl;
    std::cout<< "The GPU Clock: " << myGpu.GetClock() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout<< "My GPU utilization is : "<< myGpu.GetUtilization() << std::endl;
    myGpu.SetClock(726750000);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout<< "My GPU utilization is : "<< myGpu.GetUtilization() << std::endl;
    std::cout<< "The GPU Clock: " << myGpu.GetClock() << std::endl;
    myGpu.SetClock(1300500000);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout<< "My GPU utilization is : "<< myGpu.GetUtilization() << std::endl;
    std::cout<< "The GPU Clock: " << myGpu.GetClock() << std::endl;
    myGpu.unSetDevice();

    return 0;
}

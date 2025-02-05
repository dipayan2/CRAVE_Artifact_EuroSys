#include <assert.h>
#include <iostream>
#include <csignal>

#include "crave_governor.h"

int main(int argc, char** argv) {

    int gov_choice = 0;

    if(argc != 2){
        gov_choice = 0;
    }
    else{
        int num1 = std::atoi(argv[1]);
        gov_choice = num1;
    }


    // Lists of the permitted frequencies for the CPU, GPU, and memory
// Upload configurations!!

    // Create an instance of the governor class, and then we set the polling interval in miliseconds. And then we schedule it.
    CRAVEGovernor run_governor(0, 0, 250, cpu_cluster,mem_cluster,gpu_cluster); 
    if(gov_choice == 0){
        run_governor.ScheduleCRAVE();
    }
    else{
        std::cout<< "Incorrect Governor.. Exit"<<std::endl;
    }

    return 0;
}
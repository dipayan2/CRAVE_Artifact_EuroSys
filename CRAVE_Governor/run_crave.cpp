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
    std::vector<long long> cpu_freq_list{345600,499200,652800,806400,960000,1113600,1267200,1420800,1574400,1728000,1881600,2035200}; // Size: 8
    std::vector<long long> gpu_freq_list{114750000,216750000,318750000,420750000,522750000,624750000,726750000,854250000,930750000,1032750000,1122000000,1236750000,1300500000}; // Size: 13
    std::vector<long long> mem_freq_list{40800000,68000000,102000000,204000000,408000000,665600000,800000000,1062400000,1331200000,1600000000,1866000000}; // Size: 11


    
    // Initial Settings
    governor_settings cpu_cluster = {
        {345600, {{"gpu", 522750000}, {"mem", 800000000}}},
        {499200, {{"gpu", 318750000}, {"mem", 800000000}}},
        {652800, {{"gpu", 726750000}, {"mem", 665600000}}},
        {806400, {{"gpu", 624750000}, {"mem", 1331200000}}},
        {960000, {{"gpu", 726750000}, {"mem", 1331200000}}},
        {1113600, {{"gpu", 726750000}, {"mem", 1331200000}}},
        {1267200, {{"gpu", 726750000}, {"mem", 1331200000}}},
        {1420800, {{"gpu", 726750000}, {"mem", 1331200000}}},
        {1574400, {{"gpu", 726750000}, {"mem", 1331200000}}},
        {1728000, {{"gpu", 854250000}, {"mem", 1600000000}}},
        {1881600, {{"gpu", 930750000}, {"mem", 1600000000}}},
        {2035200, {{"gpu", 930750000}, {"mem", 1866000000}}}
        
    };

    governor_settings gpu_cluster = {
        {114750000, {{"cpu", 960000},  {"mem", 665600000}}},
        {216750000, {{"cpu", 1420800}, {"mem", 665600000}}},
        {318750000, {{"cpu", 1420800}, {"mem", 656000000}}},
        {420750000, {{"cpu", 1420800}, {"mem", 1331200000}}},
        {522750000, {{"cpu", 1420800}, {"mem", 1331200000}}},
        {624750000, {{"cpu", 1420800}, {"mem", 1331200000}}},
        {726750000, {{"cpu", 1420800}, {"mem", 1331200000}}},
        {854250000, {{"cpu", 1113600}, {"mem", 1331200000}}},
        {930750000, {{"cpu", 1267200}, {"mem", 1331200000}}},
        {1032750000,{{"cpu", 1420800}, {"mem", 1062400000}}},
        {1122000000,{{"cpu", 1267200}, {"mem", 1331200000}}},
        {1236750000,{{"cpu", 1267200}, {"mem", 1331200000}}},
        {1300500000,{{"cpu", 1420800}, {"mem", 1331200000}}}
    };

    governor_settings mem_cluster = {
        {40800000,  {{"cpu", 345600}, {"gpu", 318750000}}},
        {68000000,  {{"cpu", 499200}, {"gpu", 318750000}}},
        {102000000, {{"cpu", 806400}, {"gpu", 216750000}}},
        {204000000, {{"cpu", 960000}, {"gpu", 318750000}}},
        {408000000, {{"cpu", 1420800},{"gpu", 624750000}}},
        {665600000, {{"cpu", 1420800},{"gpu", 624750000}}},
        {800000000, {{"cpu", 1420800},{"gpu", 624750000}}},
        {1062400000,{{"cpu", 1420800},{"gpu", 726750000}}},
        {1331200000,{{"cpu", 1420800},{"gpu", 726750000}}},
        {1600000000,{{"cpu", 1420800},{"gpu", 624750000}}},
        {1866000000,{{"cpu", 1420800},{"gpu", 624750000}}}
    };

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
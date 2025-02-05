#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>

// #include <boost/lexical_cast.hpp>
// #include <boost/regex.hpp>
// #include <boost/date_time.hpp>
// #include <boost/date_time/posix_time/posix_time.hpp>

#include "cpuclock.h"

CpuClock::CpuClock(int set_cpu_id) {
    this->path_      = "/sys/devices/system/cpu";
    this->cpu_id_    = set_cpu_id;
    this->dev_count_ = 8;
    this->cpu_freq_  = 0;
    this->cpu_load  = 0.0;
    // Initializing the per-core variable for later perusal
    for (int i = 0; i < 6; i++){ 
        this->cpu_util_[i] = -1;
        this->cpu_time_[i] = 0;
        this->cpu_idle_[i] = 0;       
    }
    
}

long long CpuClock::GetClock() {
    char cpu_read[256];
    snprintf(cpu_read,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq", this->cpu_id_);
    // std::cout << "The file name for cpu_read is : " << cpu_read << std::endl;

    std::ifstream cpuread_file(cpu_read);

    if (!cpuread_file.is_open()) {
        std::cerr << "Error: Unable to open " << cpu_read << std::endl;
        return 0;
    }

    std::string value;
    std::getline(cpuread_file, value); 

    // Close the file
    cpuread_file.close();
    // Not checking for the breaking of the output, hopefully sysfs is better designed than that
    this->cpu_freq_ =  std::stoll(value); 
    return this->cpu_freq_;
}


void CpuClock::SetClock(long long new_freq) {

    // Checking if we need to modify the file at all?
    if (new_freq == this->cpu_freq_){ // If the frequency is the same as the current do not need to do anything
         return;
     }   

    // If the frequencies are different we need to modify the max and min files for the CPU min max frequency
    char cpu_min_file[256];
    char cpu_max_file[256];

    // Getting the file name
    snprintf(cpu_min_file,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_min_freq", this->cpu_id_);    
    snprintf(cpu_max_file,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_max_freq", this->cpu_id_);

    int increase_flag = (new_freq > this->cpu_freq_)? 1: 0; // The test flag is set.

    std::ofstream cpu_min(cpu_min_file);
    std::ofstream cpu_max(cpu_max_file);

    // Check if both the files were successfully opened

    if ( (!cpu_min.is_open()) || (!cpu_max.is_open())){
        std::cerr << "[CPU]Error: Unable to open the files for operation"<< std::endl;
        return;
    }

    std::string freq_val = std::to_string(new_freq);

    // Handling the file order based on the increase_flag

    if(increase_flag == 1){
        cpu_max << freq_val;
        cpu_min << freq_val;
    }
    else{
        cpu_min << freq_val;
        cpu_max << freq_val;
    }

    //Close the files

    cpu_max.close();
    cpu_min.close();

    // Setting the values based on file read.... this can be further optimized

    this->cpu_freq_ =  this->GetClock();
    return;
}

double CpuClock::GetUtilization() {
    /*
    The file for CPU Usage looks like this: 
    0 0 1197858456989 1011678547142 50156791
    1 0 1197858456995 1197574086967 0
    2 0 1197858456997 1197831394038 0
    3 0 1197858456998 1002111657919 81145230
    4 0 1197858457000 994804318858 42110913
    5 0 1197858457001 999061595664 22807188

    We need to read the two data from column 3 andf column 4 to get the idle time and then we can use them to calculate the utilization
    */
    std::ifstream cpuload("/sys/devices/system/cpu/cpufreq/cpuload/cpu_usage"); // Open the file to read data 
    if (!cpuload) {
        std::cerr << "Unable to open file!" << std::endl;
        return -1.0;
    }
    std::vector<unsigned long long> total_time_list;
    std::vector<unsigned long long> idle_time_list;

    std::string line;
	while ( std::getline(cpuload, line) ) {


        std::stringstream ss(line);  // Create string stream for parsing the line
        int id;
        unsigned long long u_val0;
        unsigned long long total_time;
        unsigned long long idle_time;
        unsigned long long u_val1;

        // Read from the string stream into the variables
        ss >> id >> u_val0 >> total_time >> idle_time >> u_val1;

        total_time_list.push_back( total_time);
        idle_time_list.push_back( idle_time);
			
	}
    cpuload.close();


    // The file has been read, updated the values for calculation, and then closed
    if (total_time_list.size() < 6){
        std::cerr << " The CPU data is incomplete" << std::endl;
        return -1.0;
    }
    double temp_load = 0.0;

    for(int idx = 0; idx < 6; idx++){
        if (this->cpu_time_[idx] == 0){
            // printf("I'm in 0 condition\n");
            this->cpu_util_[idx] = -1;
            this->cpu_time_[idx] = total_time_list[idx];
            this->cpu_idle_[idx] = idle_time_list[idx];
        }
        else{
            unsigned long long dtime = total_time_list[idx] - this->cpu_time_[idx];
            unsigned long long id_time = idle_time_list[idx] - this->cpu_idle_[idx];
            
            this->cpu_util_[idx] = 100.0 * ( 1.0 - ( (double) id_time / (double) dtime ));
            // printf("I'm in non zero condition %llu, %llu,  %lf \n",id_time, dtime, this->cpu_util_[idx] );
            this->cpu_time_[idx] = total_time_list[idx];
            this->cpu_idle_[idx] = idle_time_list[idx];
        }

        if (this-> cpu_util_[idx] > temp_load ){
            temp_load = this-> cpu_util_[idx];
        }
    }

    this->cpu_load = temp_load;
    return this->cpu_load;
}

void CpuClock::unSetDevice(std::string gov_set){
    /**
     * This code will remove the max and min settings forced by our governor, and will
     * allow the device to operate in the default kernel specified governor.
     * We will use the 'schedutil' governor as the default.
    **/
    char cpu_min_file[256];
    char cpu_max_file[256];
    char cpu_gov_file[256];

    // Getting the file name
    snprintf(cpu_min_file,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_min_freq", this->cpu_id_);    
    snprintf(cpu_max_file,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_max_freq", this->cpu_id_);
    snprintf(cpu_gov_file,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor", this->cpu_id_);

    // Creating ofstream to write to these sysfs files

    std::ofstream cpu_min(cpu_min_file);
    std::ofstream cpu_max(cpu_max_file);
    std::ofstream cpu_gov(cpu_gov_file);

    // Check if these files were successfully opened otherwise state that the operation failed

    if ( (!cpu_min.is_open()) || (!cpu_max.is_open()) || (!cpu_gov.is_open())){
        std::cerr << "[CPU]Error: Unable to open the files for unseting the device"<< std::endl;
        return;
    }

    std::string min_freq_val = "345600";
    std::string max_freq_val = "1420800";
    std::string gov_name = gov_set;

    cpu_max << max_freq_val;
    cpu_min << min_freq_val;
    cpu_gov << gov_name;

    cpu_max.close();
    cpu_min.close();
    cpu_gov.close();

    return;
}

void CpuClock::setMaxFreq( long long new_freq){
     /**
     * This will set the max frequency for this resource
    **/
    char cpu_min_file[256];
    char cpu_max_file[256];
    char cpu_gov_file[256];

    // Getting the file name
    snprintf(cpu_min_file,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_min_freq", this->cpu_id_);    
    snprintf(cpu_max_file,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_max_freq", this->cpu_id_);
    snprintf(cpu_gov_file,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor", this->cpu_id_);

    // Creating ofstream to write to these sysfs files

    std::ofstream cpu_min(cpu_min_file);
    std::ofstream cpu_max(cpu_max_file);
    std::ofstream cpu_gov(cpu_gov_file);

    // Check if these files were successfully opened otherwise state that the operation failed

    if ( (!cpu_min.is_open()) || (!cpu_max.is_open()) || (!cpu_gov.is_open())){
        std::cerr << "[CPU]Error: Unable to open the files for unseting the device"<< std::endl;
        return;
    }

    std::string min_freq_val = "345600";
    std::string max_freq_val = std::to_string(new_freq);
    std::string gov_name = "schedutil";

    cpu_min << min_freq_val;
    cpu_max << max_freq_val;
    cpu_gov << gov_name;

    cpu_max.close();
    cpu_min.close();
    cpu_gov.close();

    return;
}

void CpuClock::setMinFreq( long long new_freq){
     /**
     * This will set the min frequency for this resource
    **/
    char cpu_min_file[256];
    char cpu_max_file[256];
    char cpu_gov_file[256];

    // Getting the file name
    snprintf(cpu_min_file,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_min_freq", this->cpu_id_);    
    snprintf(cpu_max_file,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_max_freq", this->cpu_id_);
    snprintf(cpu_gov_file,256, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor", this->cpu_id_);

    // Creating ofstream to write to these sysfs files

    std::ofstream cpu_min(cpu_min_file);
    std::ofstream cpu_max(cpu_max_file);
    std::ofstream cpu_gov(cpu_gov_file);

    // Check if these files were successfully opened otherwise state that the operation failed

    if ( (!cpu_min.is_open()) || (!cpu_max.is_open()) || (!cpu_gov.is_open())){
        std::cerr << "[CPU]Error: Unable to open the files for unseting the device"<< std::endl;
        return;
    }

    std::string min_freq_val = std::to_string(new_freq);
    std::string max_freq_val = "1420800";
    std::string gov_name = "schedutil";

    cpu_max << max_freq_val;
    cpu_min << min_freq_val;
    cpu_gov << gov_name;

    cpu_max.close();
    cpu_min.close();
    cpu_gov.close();

    return;
}
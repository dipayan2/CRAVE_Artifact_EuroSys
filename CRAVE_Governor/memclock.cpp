#include "memclock.h"

MemClock::MemClock() {
    this->path_     = "/sys/kernel/debug/bpmp/debug/clk/emc/";
    this->mem_freq_ = 0;
    this->mem_util_ = 0.0;
}

long long MemClock::GetClock() {
    char mem_read[256];
    strcpy(mem_read, "/sys/kernel/debug/bpmp/debug/clk/emc/rate");
    // std::cout << "The file name for mem_read is : " << mem_read << std::endl;

    std::ifstream memread_file(mem_read);

    if (!memread_file.is_open()) {
        std::cerr << "Error: Unable to open " << mem_read << std::endl;
        return 0;
    }

    std::string value;
    std::getline(memread_file, value); 

    // Close the file
    memread_file.close();
    // Not checking for the breaking of the output, hopefully sysfs is better designed than that
    this->mem_freq_ =  std::stoll(value); 
    return this->mem_freq_;
}

void MemClock::SetClock(long long new_freq) {
    
    if (new_freq == this->mem_freq_){ // If the frequency is the same as the current do not need to do anything
         return;
    }

    char mem_mrq_file[256];
    char mem_state_file[256];
    char mem_rate_file[256];

    strcpy(mem_mrq_file, "/sys/kernel/debug/bpmp/debug/clk/emc/mrq_rate_locked");
    strcpy(mem_state_file, "/sys/kernel/debug/bpmp/debug/clk/emc/state");
    strcpy(mem_rate_file, "/sys/kernel/debug/bpmp/debug/clk/emc/rate");

    std::ofstream mem_mrq(mem_mrq_file);
    std::ofstream mem_state(mem_state_file);
    std::ofstream mem_rate(mem_rate_file);

    if ( (!mem_mrq.is_open()) || (!mem_state.is_open()) || (!mem_rate.is_open()) ){
        std::cerr << "[Mem]Error: Unable to open the files for operation"<< std::endl;
        return;
    }

    std::string freq_val = std::to_string(new_freq);

    mem_mrq << "1";
    mem_state << "1";
    mem_rate << freq_val;


    mem_mrq.close();
    mem_state.close();
    mem_rate.close();

    this->mem_freq_ =  this->GetClock();

    return;
}

double MemClock::GetUtilization() {

    char mem_util_fname[256];
    long long freq_KHz;
    
    freq_KHz = this->GetClock()/1000;  // Setting the value to KHz so it can be used to measure the utiliztion
    // std::cout << "The KHz frequency is " << freq_KHz << std::endl;
    
    strcpy(mem_util_fname, "/sys/kernel/actmon_avg_activity/mc_all");
    // std::cout << "The file name for mem_util_fname is : " << mem_util_fname << std::endl;

    std::ifstream memutil_file(mem_util_fname);

    if (!memutil_file.is_open()) {
        std::cerr << "[Mem] Error: Unable to open " << mem_util_fname << std::endl;
        return 0;
    }

    std::string value;
    std::getline(memutil_file, value); 

    // Close the file
    memutil_file.close();
    // Not checking for the breaking of the output, hopefully sysfs is better designed than that
    double temp_util = ((double)std::stoll(value) * 100.0 )/ ((double) freq_KHz);
    this->mem_util_ =  temp_util;
    return this->mem_util_;
}

void MemClock::unSetDevice(){

    char mem_mrq_file[256];
    // char mem_state_file[256];
    char mem_min_file[256];
    char mem_max_file[256];

    // Getting the file name

    strcpy(mem_mrq_file, "/sys/kernel/debug/bpmp/debug/clk/emc/mrq_rate_locked");
    // strcpy(mem_state_file, "/sys/kernel/debug/bpmp/debug/clk/emc/state");
    strcpy(mem_min_file, "/sys/kernel/debug/bpmp/debug/clk/emc/min_rate");
    strcpy(mem_max_file, "/sys/kernel/debug/bpmp/debug/clk/emc/max_rate");

    // Creating ofstream to write to these sysfs files

    std::ofstream mem_min(mem_min_file);
    std::ofstream mem_max(mem_max_file);
    std::ofstream mem_mrq(mem_mrq_file);
    // std::ofstream mem_state(mem_state_file);

    // Check if these files were successfully opened otherwise state that the operation failed

    if ( (!mem_min.is_open()) || (!mem_max.is_open()) || (!mem_mrq.is_open())){       
        std::cerr << "[Mem]Error: Unable to open the files for unseting the device"<< std::endl;
        return;
    }

    std::string min_freq_val = "40800000";
    std::string max_freq_val = "1866000000";
    std::string mrq_val = "0";

    mem_mrq << mrq_val;
    mem_max << max_freq_val;
    mem_min << min_freq_val;

    mem_mrq.close();
    mem_max.close();
    mem_min.close();
 
    return;
}

void MemClock::setMaxFreq(long long new_freq){

    char mem_mrq_file[256];
    // char mem_state_file[256];
    char mem_min_file[256];
    char mem_max_file[256];

    // Getting the file name

    strcpy(mem_mrq_file, "/sys/kernel/debug/bpmp/debug/clk/emc/mrq_rate_locked");
    // strcpy(mem_state_file, "/sys/kernel/debug/bpmp/debug/clk/emc/state");
    strcpy(mem_min_file, "/sys/kernel/debug/bpmp/debug/clk/emc/min_rate");
    strcpy(mem_max_file, "/sys/kernel/debug/bpmp/debug/clk/emc/max_rate");

    // Creating ofstream to write to these sysfs files

    std::ofstream mem_min(mem_min_file);
    std::ofstream mem_max(mem_max_file);
    std::ofstream mem_mrq(mem_mrq_file);
    // std::ofstream mem_state(mem_state_file);

    // Check if these files were successfully opened otherwise state that the operation failed

    if ( (!mem_min.is_open()) || (!mem_max.is_open()) || (!mem_mrq.is_open())){       
        std::cerr << "[Mem]Error: Unable to open the files for setMax the device"<< std::endl;
        return;
    }

    std::string min_freq_val = "40800000";
    std::string max_freq_val = std::to_string(new_freq);
    std::string mrq_val = "0";

    mem_mrq << mrq_val;
    mem_min << min_freq_val;
    mem_max << max_freq_val;

    mem_mrq.close();
    mem_max.close();
    mem_min.close();
 
    return;
}

void MemClock::setMinFreq(long long new_freq){

    char mem_mrq_file[256];
    // char mem_state_file[256];
    char mem_min_file[256];
    char mem_max_file[256];

    // Getting the file name

    strcpy(mem_mrq_file, "/sys/kernel/debug/bpmp/debug/clk/emc/mrq_rate_locked");
    // strcpy(mem_state_file, "/sys/kernel/debug/bpmp/debug/clk/emc/state");
    strcpy(mem_min_file, "/sys/kernel/debug/bpmp/debug/clk/emc/min_rate");
    strcpy(mem_max_file, "/sys/kernel/debug/bpmp/debug/clk/emc/max_rate");

    // Creating ofstream to write to these sysfs files

    std::ofstream mem_min(mem_min_file);
    std::ofstream mem_max(mem_max_file);
    std::ofstream mem_mrq(mem_mrq_file);
    // std::ofstream mem_state(mem_state_file);

    // Check if these files were successfully opened otherwise state that the operation failed

    if ( (!mem_min.is_open()) || (!mem_max.is_open()) || (!mem_mrq.is_open())){       
        std::cerr << "[Mem]Error: Unable to open the files for setMax the device"<< std::endl;
        return;
    }

    std::string min_freq_val = std::to_string(new_freq);
    std::string max_freq_val = "1866000000";
    std::string mrq_val = "0";

    mem_mrq << mrq_val;
    mem_max << max_freq_val;
    mem_min << min_freq_val;
 

    mem_mrq.close();
    mem_max.close();
    mem_min.close();
 
    return;
}
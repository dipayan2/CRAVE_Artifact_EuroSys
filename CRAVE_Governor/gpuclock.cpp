#include "gpuclock.h"

GpuClock::GpuClock() {
    this->path_     = "/sys/devices/gpu.0/";
    this->gpu_freq_ = 0;
    this->gpu_util_ = 0.0;
}

long long GpuClock::GetClock() {
    char gpu_read[256];
    strcpy(gpu_read, "/sys/devices/gpu.0/devfreq/17000000.gp10b/cur_freq");

    std::ifstream gpuread_file(gpu_read);

    if (!gpuread_file.is_open()) {
        std::cerr << "Error: Unable to open " << gpu_read << std::endl;
        return 0;
    }

    std::string value;
    std::getline(gpuread_file, value); 

    // Close the file
    gpuread_file.close();
    // Not checking for the breaking of the output, hopefully sysfs is better designed than that
    this->gpu_freq_ =  std::stoll(value); 
    return this->gpu_freq_;
}

void GpuClock::SetClock(long long new_freq) {
  
    // Checking if we need to modify the file at all?
    if (new_freq == this->gpu_freq_){ // If the frequency is the same as the current do not need to do anything
         return;
     }   

    // If the frequencies are different we need to modify the max and min files for the CPU min max frequency
    char gpu_min_file[256];
    char gpu_max_file[256];

    // Getting the file name
    strcpy(gpu_max_file, "/sys/devices/17000000.gp10b/devfreq/17000000.gp10b/max_freq");
    strcpy(gpu_min_file, "/sys/devices/17000000.gp10b/devfreq/17000000.gp10b/min_freq");

    int increase_flag = (new_freq > this->gpu_freq_)? 1: 0; // The test flag is set.

    std::ofstream gpu_min(gpu_min_file);
    std::ofstream gpu_max(gpu_max_file);

    // Check if both the files were successfully opened

    if ( (!gpu_min.is_open()) || (!gpu_max.is_open())){
        std::cerr << "[GPU]Error: Unable to open the files for operation"<< std::endl;
        return;
    }

    std::string freq_val = std::to_string(new_freq);

    // Handling the file order based on the increase_flag

    if(increase_flag == 1){
        gpu_max << freq_val;
        gpu_min << freq_val;
    }
    else{
        gpu_min << freq_val;
        gpu_max << freq_val;
    }

    //Close the files

    gpu_max.close();
    gpu_min.close();

    // Setting the values based on file read.... this can be further optimized

    this->gpu_freq_ =  this->GetClock();
    return;
}

double GpuClock::GetUtilization() {
    char gpu_util_fname[256];

    strcpy(gpu_util_fname, "/sys/devices/17000000.gp10b/load");
    // std::cout << "The file name for mem_util_fname is : " << mem_util_fname << std::endl;

    std::ifstream gpuutil_file(gpu_util_fname);

    if (!gpuutil_file.is_open()) {
        std::cerr << "Error: Unable to open " << gpu_util_fname << std::endl;
        return 0;
    }

    std::string value;
    std::getline(gpuutil_file, value); 

    // Close the file
    gpuutil_file.close();
    // Not checking for the breaking of the output, hopefully sysfs is better designed than that
    double temp_util = ((double)std::stoll(value) / 10.0);
    this->gpu_util_ =  temp_util;
    return this->gpu_util_;
}

void GpuClock::unSetDevice(std::string gov_str){
    /**
     * This code will remove the max and min settings forced by our governor, and will
     * allow the device to operate in the default kernel specified governor.
     * We will use the 'nvhost_podgov' governor as the default.
    **/
    char gpu_min_file[256];
    char gpu_max_file[256];
    char gpu_gov_file[256];

    // Getting the file name
    strcpy(gpu_max_file, "/sys/devices/17000000.gp10b/devfreq/17000000.gp10b/max_freq");
    strcpy(gpu_min_file, "/sys/devices/17000000.gp10b/devfreq/17000000.gp10b/min_freq");
    strcpy(gpu_gov_file, "/sys/devices/17000000.gp10b/devfreq/17000000.gp10b/governor");

    // Creating ofstream to write to these sysfs files

    std::ofstream gpu_min(gpu_min_file);
    std::ofstream gpu_max(gpu_max_file);
    std::ofstream gpu_gov(gpu_gov_file);

    // Check if these files were successfully opened otherwise state that the operation failed

    if ( (!gpu_min.is_open()) || (!gpu_max.is_open()) || (!gpu_gov.is_open())){
        std::cerr << "[GPU]Error: Unable to open the files for unseting the device"<< std::endl;
        return;
    }

    std::string min_freq_val = "114750000";
    std::string max_freq_val = "1300500000";
    std::string gov_name = gov_str;

    gpu_max << max_freq_val;
    gpu_min << min_freq_val;
    gpu_gov << gov_name;

    gpu_max.close();
    gpu_min.close();
    gpu_gov.close();

    return;
}

void GpuClock::setMaxFreq(long long new_freq){
    /**
     * Sets the max value
    **/
    char gpu_min_file[256];
    char gpu_max_file[256];
    char gpu_gov_file[256];

    // Getting the file name
    strcpy(gpu_max_file, "/sys/devices/17000000.gp10b/devfreq/17000000.gp10b/max_freq");
    strcpy(gpu_min_file, "/sys/devices/17000000.gp10b/devfreq/17000000.gp10b/min_freq");
    strcpy(gpu_gov_file, "/sys/devices/17000000.gp10b/devfreq/17000000.gp10b/governor");

    // Creating ofstream to write to these sysfs files

    std::ofstream gpu_min(gpu_min_file);
    std::ofstream gpu_max(gpu_max_file);
    std::ofstream gpu_gov(gpu_gov_file);

    // Check if these files were successfully opened otherwise state that the operation failed

    if ( (!gpu_min.is_open()) || (!gpu_max.is_open()) || (!gpu_gov.is_open())){
        std::cerr << "[GPU]Error: Unable to open the files for unseting the device"<< std::endl;
        return;
    }

    std::string min_freq_val = "114750000";
    std::string max_freq_val = std::to_string(new_freq);
    std::string gov_name = "nvhost_podgov";

    gpu_min << min_freq_val;
    gpu_max << max_freq_val;
    gpu_gov << gov_name;

    gpu_max.close();
    gpu_min.close();
    gpu_gov.close();

    return;
}

void GpuClock::setMinFreq(long long new_freq){
    /**
     * Sets the max value
    **/
    char gpu_min_file[256];
    char gpu_max_file[256];
    char gpu_gov_file[256];

    // Getting the file name
    strcpy(gpu_max_file, "/sys/devices/17000000.gp10b/devfreq/17000000.gp10b/max_freq");
    strcpy(gpu_min_file, "/sys/devices/17000000.gp10b/devfreq/17000000.gp10b/min_freq");
    strcpy(gpu_gov_file, "/sys/devices/17000000.gp10b/devfreq/17000000.gp10b/governor");

    // Creating ofstream to write to these sysfs files

    std::ofstream gpu_min(gpu_min_file);
    std::ofstream gpu_max(gpu_max_file);
    std::ofstream gpu_gov(gpu_gov_file);

    // Check if these files were successfully opened otherwise state that the operation failed

    if ( (!gpu_min.is_open()) || (!gpu_max.is_open()) || (!gpu_gov.is_open())){
        std::cerr << "[GPU]Error: Unable to open the files for unseting the device"<< std::endl;
        return;
    }

    std::string min_freq_val = std::to_string(new_freq);
    std::string max_freq_val = "1300500000";
    std::string gov_name = "nvhost_podgov";

    gpu_max << max_freq_val;
    gpu_min << min_freq_val;
    gpu_gov << gov_name;

    gpu_max.close();
    gpu_min.close();
    gpu_gov.close();

    return;
}
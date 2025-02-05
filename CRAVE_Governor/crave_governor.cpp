#include "crave_governor.h"
#include <iostream>
#include <csignal>

using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::chrono::time_point;
using std::this_thread::sleep_until;

volatile sig_atomic_t stopGov = 0;

void handleClose(int sig){
    std::cout<<"[CRAVE] Handling the signal handler" << std::endl;
    stopGov = 1;
}

CRAVEGovernor::CRAVEGovernor(int set_cpu_id, int set_gpu_id, int set_polling_time, governor_settings c_clust, governor_settings m_clust, governor_settings g_clust)
    : cpu_man_(set_cpu_id) {
    
    this->cpu_id_       = set_cpu_id;
    this->gpu_id_       = set_gpu_id; // Not used in this analysis
    this->polling_time_ = set_polling_time;
    this->cpu_cluster_  = c_clust;
    this->mem_cluster_  = m_clust;
    this->gpu_cluster_  = g_clust;
    this->max_freq[CPU] = 1420800;
    this->max_freq[MEM] = 1866000000;
    this->max_freq[GPU] = 1300500000;
    // Set the Resource Impact matrix for our analysis

    this->RI[CPU][CPU] = 0.75; this->RI[CPU][MEM] = 0.36; this->RI[CPU][GPU] = 0.01;
    this->RI[MEM][CPU] = 0.48; this->RI[MEM][MEM] = 0.87; this->RI[MEM][GPU] = 0.69;
    this->RI[GPU][CPU] = 0.00; this->RI[GPU][MEM] = 0.00; this->RI[GPU][GPU] = 0.49;
}

void CRAVEGovernor::getDominantResource(double (&Utility)[3]){
    // Get the utilzation of the three resources

    double util_[3];
    long long cur_freq[3];

    util_[CPU] = this->cpu_man_.GetUtilization(); 
    cur_freq[CPU] = this->cpu_man_.GetClock();
    util_[MEM] = this->mem_man_.GetUtilization();
    cur_freq[MEM] = this->mem_man_.GetClock();
    util_[GPU] = this->gpu_man_.GetUtilization();
    cur_freq[GPU] = this->gpu_man_.GetClock();

    // Get the cost of the resource
    // Make some slight order change for the dom resource based on utilization
    Utility[CPU] = 0.0;
    Utility[MEM] = 0.0;
    Utility[GPU] = 0.0;

    // In case of extreme usage set the corresponding resource as the DOM resource 
    // Otherwise we can get into resource thrashing, where we can never become the max resource

    // if (util_[MEM] > 90.0){
    //     Utility[MEM] = 100.0;
    //     return;
    // }
    // else if(util_[CPU] > 95.0){
    //     Utility[CPU] = 100.0;
    //     return;
    // }
    if(util_[GPU] > 90.0){
        Utility[GPU] = 100.0;
        return;
    }

    double cost_[3];

    for (int dev = CPU; dev < OTH; ++dev ){
        cost_[dev] = (util_[dev] * (double) cur_freq[dev])/((double) this->max_freq[dev]);
    }

    // double Utility[3]; // We will use the value sent by reference

    for (int dev = CPU; dev < OTH ; ++dev ){
        Utility[dev] = cost_[CPU] * this->RI[dev][CPU] + cost_[MEM] * this->RI[dev][MEM] + cost_[GPU] * this->RI[dev][GPU];
    }

    return;
}

void CRAVEGovernor::getCost(double (&Utility)[3]){
    // Get the utilzation of the three resources

    double util_[3];
    long long cur_freq[3];

    util_[CPU] = this->cpu_man_.GetUtilization(); 
    cur_freq[CPU] = this->cpu_man_.GetClock();
    util_[MEM] = this->mem_man_.GetUtilization();
    cur_freq[MEM] = this->mem_man_.GetClock();
    util_[GPU] = this->gpu_man_.GetUtilization();
    cur_freq[GPU] = this->gpu_man_.GetClock();

    // Get the cost of the resource

    double cost_[3];

    for (int dev = CPU; dev < OTH; ++dev ){
        Utility[dev] = (util_[dev] * (double) cur_freq[dev])/((double) this->max_freq[dev]);
    }
    Utility[MEM] = 0.0;
    return;
}

void CRAVEGovernor::SetPolicyCpuFreq() {
    long long cpu_freq = this->cpu_man_.GetClock();

    std::cout << "CPU utilization is " << this->cpu_man_.GetUtilization() << std::endl;

    long long gpu_freq = this->cpu_cluster_[cpu_freq]["gpu"];
    long long mem_freq = this->cpu_cluster_[cpu_freq]["mem"];

    this->gpu_man_.SetClock(gpu_freq);
    this->mem_man_.SetClock(mem_freq);

    return;
}

void CRAVEGovernor::SetPolicyGpuFreq() {
    long long gpu_freq = this->gpu_man_.GetClock();

    long long cpu_freq = int(this->gpu_cluster_[gpu_freq]["cpu"]);
    long long mem_freq = int(this->gpu_cluster_[gpu_freq]["mem"]);

    this->cpu_man_.SetClock(cpu_freq);
    this->mem_man_.SetClock(mem_freq);

    return;
}

void CRAVEGovernor::SetPolicyMemFreq() {
    long long mem_freq = this->mem_man_.GetClock();

    std::cout << "Memory utilization is " << this->mem_man_.GetUtilization() << std::endl;

    long long cpu_freq = int(this->mem_cluster_[mem_freq]["cpu"]);
    long long gpu_freq = int(this->mem_cluster_[mem_freq]["gpu"]);

    this->cpu_man_.SetClock(cpu_freq);
    this->gpu_man_.SetClock(gpu_freq);
    return;
}

void CRAVEGovernor::SetPolicyCRAVE(){
    // Set a vector which will have the dominant resource of the system
    double Utility[3] = {0.0, 0.0, 0.0};
    // Get the utility vector which will be used to get the dominant reosurce of the system
    this->getDominantResource(Utility);

    // We will use the Utility metric to get the actual dominant reosurce
    double maxUtil = Utility[CPU];
    int maxArg = CPU;

    for (int dev = CPU ; dev < OTH; ++dev){
        if (Utility[dev] > maxUtil){
            maxUtil = Utility[dev];
            maxArg = dev;
        }
    }
    std::cout << "[To be removed] The dominant resource is "<< maxArg << std::endl;

    // unSet the max resource and then set the frequenchy of the rest using the relevant value

    switch(maxArg){
        case CPU:
        {
            // Use the CPU as the dominant resource to set other devices
            std::cout << "CPU is the dominant resource" << std::endl;
            this->cpu_man_.unSetDevice();
            long long cpu_f = this->cpu_man_.GetClock();
            // Get the clock of the other devices
            long long gpu_f = this->cpu_cluster_[cpu_f]["gpu"];
            long long mem_f = this->cpu_cluster_[cpu_f]["mem"];
            this->gpu_man_.SetClock(gpu_f);
            this->mem_man_.SetClock(mem_f);
            break;
        }
        case MEM:
        {
            // Use the Mem as the dominant resource to set other devices
            std::cout << "Mem is the dominant resource" << std::endl;
            this->mem_man_.unSetDevice();
            long long mem_f = this->mem_man_.GetClock();
            // Get the clock of the other devices
            long long cpu_f = this->mem_cluster_[mem_f]["cpu"];
            long long gpu_f = this->mem_cluster_[mem_f]["gpu"];
            this->cpu_man_.SetClock(cpu_f);
            this->gpu_man_.SetClock(gpu_f);
            break;
        }
        case GPU:
        {
            // Use the GPU as the dominant resource to set other devices
            std::cout << "GPU is the dominant resource" << std::endl;
            this->gpu_man_.unSetDevice();
            long long gpu_f = this->gpu_man_.GetClock();
            // Get the clock of the other devices
            long long cpu_f = this->gpu_cluster_[gpu_f]["cpu"];
            long long mem_f = this->gpu_cluster_[gpu_f]["mem"];
            this->cpu_man_.SetClock(cpu_f);
            this->mem_man_.SetClock(mem_f);
            break;
        }
        default:
            // Don't do anything, wait for the next cycle
            std::cout<<"[ERR] Could not find a dominant resource!!!!" << std::endl;
            break;
    }

    return;
}



void CRAVEGovernor::SetCluster(governor_settings new_cluster) {
    this->other_cluster_ = new_cluster;
}

void CRAVEGovernor::ScheduleCRAVE() {
    std::cout << "Starting the Scheduler" << std::endl;
    std::signal(SIGINT, handleClose);

    while (true) {
    	time_point<steady_clock> start = steady_clock::now();
        time_point<steady_clock> wake_time = start + std::chrono::milliseconds(this->polling_time_);
        
        // this->SetPolicyCpuUtil();
        this->SetPolicyCRAVE();
        sleep_until(wake_time);
        
        duration<double> elapsed_seconds = steady_clock::now() - start;
        // Want overhead of 10 milliseconds or less. The new overhead is less than .12 ms
        std::cout << "Time elapsed for one cycle: " << elapsed_seconds.count() << std::endl;
         if (stopGov == 1){
            this->cpu_man_.unSetDevice();
            this->mem_man_.unSetDevice();
            this->gpu_man_.unSetDevice();
            std::cout << "[GOV] Unsetting all the devices" << std::endl;
            break;
         }
    }
}


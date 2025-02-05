#pragma once

#include <chrono>
#include <unordered_map>
#include <thread>

#include "cpuclock.h"
#include "gpuclock.h"
#include "memclock.h"

/**
 * This unordered map allows us to set the 
 * 
 * 
 * **/ 
typedef std::unordered_map<long long, std::unordered_map<std::string, long long>> governor_settings;
enum DomR {
    CPU,
    MEM,
    GPU,
    OTH
};

class CRAVEGovernor {
private:
public:
    // Move to private
    int               cpu_id_; 
    int               gpu_id_; 
    int               polling_time_; //vs polling_s???? What unit is this in???
    CpuClock          cpu_man_; // https://stackoverflow.com/a/12927220 Initialize member objects!!!
    GpuClock          gpu_man_;
    // std::thread       my_thread_; //https://stackoverflow.com/a/25525465
    MemClock          mem_man_;

    long long max_freq[3];

    double RI[3][3];

    governor_settings cpu_cluster_;
    governor_settings mem_cluster_;
    governor_settings gpu_cluster_;
    governor_settings other_cluster_;
    // We need to set the max values of the each of the resoruces.

    // Allow setting cluster here????
    /**
     * Creates an CRAVE governor object containing a cpu_id_ of set_cpu_id,
     * a CpuClock with a cpu_id_ of set_cpu_id, a gpu_id_ of set_gpu_id
     * (even though GpuClock has no gpu_id_ member variable), a polling_time_
     * of set_polling_time, a GpuClock and a MemClock created with the default
     * constructors, and a a cluster_ of cluster
     * 
     * @param set_cpu_id       Set cpu_id_ to this value.
     *     Also makes cpu_man_ have this value as its cpu_id_.
     * @param set_gpu_id       Set gpu_id_ to this value.
     * @param set_polling_time Set polling_time_ to this value.
     * @param set_cluster      Set cluster_ to this value.
     *
     */
    CRAVEGovernor(int set_cpu_id, int set_gpu_id, int set_polling_time, governor_settings c_clust,governor_settings m_clust,governor_settings g_clust);

    /** 
     * This function is used to determine the dominant resource of the system
     * **/
    void getDominantResource(double (&Utility)[3]);
    /** 
     * This function is used to determine the max cost resource of the system
     * **/
    void getCost(double (&Utility)[3]);
    
    /**
     * We select the best configuration for all resources based
     * on the current CPU frequency. The CPU freq is chosen by the default governor
     * 
     */
    void SetPolicyCpuFreq();

    /**
     * We select the best configuration for all resources based
     * on the current GPU frequency. The GPU frequency is chosen by the default governor
     * 
     */
    void SetPolicyGpuFreq();

    /**
     * We select the best configuration for all resources based
     * on the current memory frequency
     * 
     */
    void SetPolicyMemFreq();

    /**
     * We select the best configuration for all resources based
     * on the current CPU utilization
     * 
     */
    void SetPolicyCpuUtil();

    /**
     * We select the best configuration for all resources based
     * on the current memory utilization
     * 
     */
    void SetPolicyMemUtil();

    /**
     * The fuction void setCRAVE is our actual policy where we find the dominant resource and then use it to set the freq
    **/
    void SetPolicyCRAVE();       


    /**
     * The fuction void setCRAVE is our actual policy where we find the cost resource and then use it to set the freq
    **/
    void SetPolicyCoCAP();  

    /** 
     * Creating the policy for GEAR DVFS
     * **/
    void SetPolicyGEAR(int state = 0);
    /**
     * Setter for cluster_
     * 
     * @param set_cluster      Set cluster_ to this value.
     *
     */
    void SetCluster(governor_settings new_cluster);
    
    /**
     * Sets frequencies based on the current cluster_ 
     * every polling_time_ milliseconds
     *
     */
    void ScheduleCRAVE();
        /**
     * Sets frequencies based on the current cluster_ 
     * every polling_time_ milliseconds
     *
     */
};
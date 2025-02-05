#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>

/**
 * An class for representing the clock of a CPU
 *
 */
class CpuClock {
private:
public:
    // TODO: MOVE TO PRIVATE
	std::string path_;
    int         cpu_id_;
    int         dev_count_; // The number of CPU cores
	long long   cpu_freq_;
	double      cpu_util_[6];
	unsigned long long 	cpu_time_[6];
	unsigned long long 	cpu_idle_[6];
	double 		cpu_load;

    /**
	 * Creates a CpuClock object with a path_ of "/sys/devices/system/cpu",
	 * a cpu_id_ equal to set_cpu_id, a dev_count_ of 8, a cpu_freq_ of 0,
     * and a cpu_util_ of 0.0
     * 
     * @param set_cpu_id Set cpu_id_ to this value.
	 *
	 */
    CpuClock(int set_cpu_id = 0);

	/**
	 * Gets the current frequency for this GPU, stores it in cpu_freq_, and returns it
	 *
	 * @return The current CPU frequency
	 */
    long long GetClock();

	/**
	 * Sets this CPU's frequency
	 *
	 * @param new_freq We set the CPU frequency to this value and store it in cpu_freq_
	 *				   
	 */
    void SetClock(long long new_freq);

	// std::vector<long long> GetAllClock(); // Not implemented

	void unSetDevice(std::string gov_set = "schedutil"); // This function will set this device to our chosen governor settings, and then allow it move freely

	void setMaxFreq( long long new_freq); // To set the cap of the frequency

	void setMinFreq(long long new_freq); // To set the min cap of the frequency
	
	double GetUtilization(); // Will be impleneted in this version
};
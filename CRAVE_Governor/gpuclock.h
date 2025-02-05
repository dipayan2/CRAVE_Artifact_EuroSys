#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdlib.h>

// #include <boost/lexical_cast.hpp>
// #include <boost/regex.hpp>
// #include <boost/date_time.hpp>
// #include <boost/date_time/posix_time/posix_time.hpp>

/**
 * An class for representing the clock of a GPU
 *
 */
class GpuClock {
private:
public:
	// MOVE TO PRIVATE!!!
	std::string path_;
	long long   gpu_freq_;
	double      gpu_util_;

	/**
	 * Creates a GpuClock object with a path_ of "/sys/devices/system/cpu",
	 * a gpu_freq_ of 0, and a gpu_util_ of 0.0.
	 *
	 */
	GpuClock();

	/**
	 * Gets the current frequency for the GPU, stores it in gpu_freq_, and returns it
	 *
	 * @return The current GPU frequency
	 */
	long long GetClock(); //What is type of gpu_freq????? string???? -> we get all but last character
	
	/**
	 * Sets the GPU's frequency
	 *
	 * @param new_freq We set the GPU frequency to this value and store it in gpu_freq_
	 */
	void SetClock(long long new_freq);

	std::vector<long long> GetAllClock(); // Not implemented

	void unSetDevice(std::string gov_str = "nvhost_podgov"); // This function will set this device to our chosen governor settings, and then allow it move freely

	void setMaxFreq( long long new_freq); // To set the cap of the frequency

	void setMinFreq(long long new_freq); // To set the min cap of the frequency
	
	double GetUtilization();
};

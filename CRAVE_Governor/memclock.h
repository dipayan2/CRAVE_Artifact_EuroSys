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
 * An class for representing the clock of a memory
 *
 */
class MemClock {
private:
public:
	// Move to private
	std::string path_;
	long long   mem_freq_;
	double      mem_util_;

	/**
	 * Creates a MemClock object with a path_ of "/sys/devices/system/cpu",
	 * a mem_freq_ of 0, and a mem_util_ of 0.
	 *
	 */
	MemClock();

	/**
	 * Gets the current frequency for the memory, stores it in mem_freq_, and returns it
	 *
	 * @return The current memory frequency
	 */
	long long GetClock();
	
	/**
	 * Sets the memory's frequency
	 *
	 * @param new_freq We set the memory frequency to this value and store it in mem_freq_
	 *				   
	 */
	void SetClock(long long new_freq);

	std::vector<long long> GetAllClock(); // This won't be implemented

	void unSetDevice(); // This function will set this device to our chosen governor settings, and then allow it move freely

	void setMaxFreq( long long new_freq); // To set the cap of the frequency

	void setMinFreq(long long new_freq); // To set the min cap of the frequency
	
	double GetUtilization(); // We will use the formatting from the CPU style values to get this going.
};
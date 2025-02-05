# OBJS = profile_dvfs.o ols_scheduler.o cpuclock.o gpuclock.o memclock.o
# OBJS = test_code.o cpuclock.o memclock.o gpuclock.o crave_governor.o
OBJS = run_crave.o cpuclock.o memclock.o gpuclock.o crave_governor.o
# HEADER = ols_scheduler.h cpuclock.h gpuclock.h memclock.h
HEADER = cpuclock.h memclock.h gpuclock.h crave_governor.h
OUT = run_crave
# OUT = test_code
CXX = g++
CXXFLAGS = -g -O2 -Wall -Wextra -pedantic 

all : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(OUT) -pthread

run_crave.o : run_crave.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c run_crave.cpp -pthread

crave_governor.o : crave_governor.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c crave_governor.cpp -pthread

test_code.o : test_code.cpp $(HEADER)
	@echo "Needs to be in Jetson TX2"
	$(CXX) $(CXXFLAGS) -c test_code.cpp -pthread

cpuclock.o : cpuclock.cpp cpuclock.h
	@echo "Needs to be in Jetson TX2"
	$(CXX) $(CXXFLAGS) -c cpuclock.cpp -pthread

gpuclock.o : gpuclock.cpp gpuclock.h
	@echo "Needs to be in Jetson TX2"
	$(CXX) $(CXXFLAGS) -c gpuclock.cpp -pthread

memclock.o : memclock.cpp memclock.h
	@echo "Needs to be in Jetson TX2"
	$(CXX) $(CXXFLAGS) -c memclock.cpp -pthread

clean:
	rm -f $(OBJS) $(OUT)

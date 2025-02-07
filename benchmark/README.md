# Install evaluation benchmark
## Installing chai benchmark
You can find the detailed instructions here [chai benchmarks](https://chai-benchmarks.github.io/gettingstarted/).
Note: You would need OpenCL installed in your system
### For Jetson TX2
Step1. Install the git repo and set the encironment variables
Step2. ``` cd CUDA-D ```
Step3. Choose the benchmark ``` cd SSSP ```
Step4. Compile and execute
```
make
time ./sssp

```
For more details follow the instruction in the webpage

### For ODRODXU-4
Step1. Install the repo and the environment variables
Step2. ``` cd OpenCL-D ```
Follow Step 3 and 4 from above

## Installing RODINIA benchmark
You can find the detailed instruction for Rodinia [here] (https://github.com/yuhc/gpu-rodinia).

Use the appropriate folder for evaluation
1. For Jetson TX2 use ``` cuda ```
2. For ODROID XU4 use  ``` opencl ```


# Evaluating the governor

We will collect the following data:
1. Execution time for performance
2. Power (if your device has internal power monitors)

## Collecting performane
```
Open the particular benchmark foler of your choice(for example SSSP), as shown in previous section

cd chai/CUDA-D/SSSP/
time ./sssp

```
This will provide the execution time

## Measuring power and energy
Jetson TX2 has internal power meters, which allows us to measure the power and the energy consumed during the whole process

Step1. Start the `tegrastats` 
```
sudo -b tegrastats --interval 100 --logfile <FileName>

```
Step2. After the end of the benchmark

```
sudo -b tegrastats --stop
```



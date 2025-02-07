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

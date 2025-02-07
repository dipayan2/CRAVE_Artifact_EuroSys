# CRAVE Governor

## Compilation

To compile the governor, you need to run the make file
```
make
```

This should compile the governor and generate the output `run_crave` which is the binary for our governor

## Running the governor

In a new terminal run this 
```
sudo taskset -c <cpu_id> ./run_crave

```
We choose a particular cpu_id to prevent the governor from impacting the performance of the benchmarks





![example workflow](https://github.com/Nosenzor/Container_ShowCase/actions/workflows/lukka_ci.yml/badge.svg)

# Training C++ Containers

## Intro :
* This repo aims to show some famous containers and to help beginners to pick the best container for their problem
* **Always measure and profile to understand what is your performance problem**

## Building examples

Tests are built through the cmake command through CMakeList.txt


## Running tests
* CTest way :
```
ctest
```

* Boost Test / List all Tests
```
./training_containers  --list_content
```

* Boost Test / Run All Tests
```
./training_containers --log_level=all
```
* Boost Test / Run Some tests 
```
./training_containers --run_test=array --log_level=all 
./training_containers --run_test=vec --log_level=all 
./training_containers --run_test=deque --log_level=all 
./training_containers --run_test=list --log_level=all 
./training_containers --run_test=set --log_level=all 
./training_containers --run_test=uset --log_level=all
./training_containers --run_test=stable_vector --log_level=all
./training_containers --run_test=static_vector --log_level=all
./training_containers --run_test=small_vector --log_level=all
```

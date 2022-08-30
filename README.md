# TBB for DragonR 
Threading Building Blocks

## Help :
* Contact me : 
* [TBB Help (intel)](https://www.threadingbuildingblocks.org/docs/help/index.htm)

## Keep in mind :
*Always measure and profile to understand what is your performance problem*

## Building examples

Tests are built through the cmake command through CMakeList.txt hierarchy of files and subdirectory so tests are built Every time you call the master CMakeList.txt. [cf How to build](../../../../README.md)

The building script has been modified to build the tbb test cases.
cmake --build ./build --config Release -j 8

## Running tests

from the root/master directory you can launch :
### Launch all tests
First Move to the exe directory
cd build/Containers/

List all Tests
```
./Containers_tests  --list_content
```

Run All Tests
```

./Containers_tests --log_level=all
```
Run Some tests 
```
./Containers_tests --run_test=array --log_level=all 
./Containers_tests --run_test=vec --log_level=all 
./Containers_tests --run_test=deque --log_level=all 
./Containers_tests --run_test=list --log_level=all 
./Containers_tests --run_test=set --log_level=all 
./Containers_tests --run_test=uset --log_level=all
./Containers_tests --run_test=stable_vector --log_level=all
./Containers_tests --run_test=static_vector --log_level=all
./Containers_tests --run_test=small_vector --log_level=all
```

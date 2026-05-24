# qinmo_net  
- This is my net library. Implementation based on C++11 and Reactor model.  
- Reference [muduo](https://github.com/chenshuo/muduo).  
## Tutorials:  
## Install:  
After obtaining the code, go back to your project root directory and run CMakeLists.txt script:
```shell
mkdir build && cd build
cmake .. # -DQINMO_LOG_PATH="log output path" -DQINMO_LOG_LEVEL=0~5 # output level: 0:trace - 5:fatal
make
```
How to get the code:  
### Method 1:  
────If you want to compile this library, views follow the steps:  
If your project structure as follows:  
```
project/
├── external/       # third-party
├── include/
├── src/
└── CMakeLists.txt
```
You can `cd` into `external/` folder and run `git clone https://github.com/qinmoM/qinmo_net` to clone this repository(but might get work-in-progress code), or download and unpack the a version from release. Then add the following lines to the end of your *CMakeLists.txt*:  
```cmake
add_subdirectory(external/qinmo_net)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        qinmo_net
)
```
### Method 2:  
────If you want to use the library file, views follow the steps:  
---  
> Only supports linux now.  
> Welcome to discuss.  
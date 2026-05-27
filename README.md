# qinmo_net  
- This is my net library. Implementation based on C++11 and Reactor model.  
- Reference [muduo](https://github.com/chenshuo/muduo).  
## Tutorials:  
Simple example:
```cpp
#include <qinmo/net.h>

int main()
{
    qinmo::net::EventLoop loop;

    qinmo::net::InetAddr addr;
    // addr.setIP(...);
    // addr.setPort(...);
    qinmo::net::ReactorTcpServer server(&loop, addr, 0);
    // server.setConnectFunc(...);
    // server.setDisconnectFunc(...);

    server.start();
    loop.loop();

    return 0;
}
```
[See details](./example/).  
## Install:  
After obtaining the code, go back to your project root directory and run CMakeLists.txt script:
```shell
mkdir build && cd build
cmake .. # -DQINMO_LOG_PATH="log output path" -DQINMO_LOG_LEVEL=0~5 # output level: 0:trace - 5:fatal
make
```
If your project structure as follows:  
```
project/
├── external/       # third-party
├── include/
├── src/
└── CMakeLists.txt
```
How to get the code:  
### Method 1:  
────If you want to compile this library, views follow the steps:  
- You can `cd` into `external/` folder and run `git clone -b main --single-branch https://github.com/qinmoM/qinmo_net` to clone this repository, or download and unpack the a version from release.  
- Then add the following lines to the end of your *CMakeLists.txt*:  
```cmake
add_subdirectory(external/qinmo_net)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        qinmo_net
)
```
### Method 2:  
────If you want to use the library file directly, views follow the steps:  
---  
> Only supports linux now.  
> Welcome to discuss.  
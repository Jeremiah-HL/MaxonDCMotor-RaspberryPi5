# ReadMe
这是一个使用树莓派以及Maxon直流电机构建的项目，用以操控多自由度的机器人。
使用的设备是树莓派5，EPOS4 CAN Compact以及Maxon DC motor。如果你有一个DC电机，那么电机的型号任意即可。
使用C++编程语言，使用了wiringPi库，EposCmd库来构建项目。
- wiringPi：我的版本是3.01，我推荐使用github上的源码进行安装构建。
- EposCmd：这个库是由瑞士Maxon来提供的，具体库既可以在互联网搜索获得，也可以参考我的MaxonDCMotor-JetsonNano这个仓库中的一个文件夹（EPOS-Linux-xxx）来获取

这个项目的结构解释：
- lib：运行或构建所需要的动态库文件
- src：项目的源文件
- include：项目的头文件
- build：配合CMakeLists.txt文件构建C++项目，使用这个文本文件构建项目时，建议先进入build文件夹
- CMakeLists.txt

这个项目能为你提供一个面向对象的操控Maxon直流电机的库，头文件和源文件的名称为MaxonDCMotor，它继承了Motor这个抽象类，这意味着你可以自己构建一个交流电机库，也可以直接使用本项目的MaxonDCMotor类来实现你的目的。

PS：
- 由于没有使用复杂的算法，代码的注释非常少
- 其次，在openDevice等一系列方法（构造函数）中，函数的参数可以改为string类型的数据（这可能和C++的版本有关）
- - 如果修改为string类型的对象，请向以下方法向其中添加代码，转换为char*类型的数据，才能传入给VCS_OpenDevice函数：
#include <cstring>
char* pDeviceName = new char[255];
strcpy(pDeviceName, deviceName.c_str());
...
delete[] pDeviceName;

# 🧀🧀 mrcheese's internal workings 🧀🧀
## pre-requesites
```
git clone --recurse-submodules https://github.com/Ruloco967/CANDIDATES2023
brew install stlink cmake make gcc-arm-embedded
```
## compile code
```
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cubemx.cmake/arm-gcc.cmake ..
make
```

## upload code
*connect st link to stm32*
```
make flash
```

## todo
### en orden de importancia \/ \/ \/
- [X] setup stm32 programming environment
    - [X] STM32CUBEMX ioc file generator
    - [X] generate cmake project
        - [X] test debugger with st-link or openocd utilities
            - [ ] test GDB Dashboard
    
- [ ] tcs3200 sensor library
    - [stm32 code reference](https://github.com/jaimelaborda/TCS3200_STM32F4_Library/wiki)
    - [X] test printf with uart
    - [X] check `HAL_TIM_IC_CaptureCallback` works on different file (new arduino?)
    - [ ] modify parameters respecting the sensor
        - [ ] prescaler: 84
        - [ ] period: 19999
        - [ ] filter: 5
            - [ ] check why these numbers exactly
    - [ ] code for 4 parameters of sensor
    - [ ] frequency to color conversion
    - [ ] test sensor

- [X]  mpu6050 sensor driver library (check)

- [X] redirect printf to UART (check)

- [ ] configure c on ide

- [ ] lcd screen driver library
- [ ] ultrasonic sensor driver library
- [ ] motor driver library
- [ ] encoder driver library

- [ ] velocity PID

- [ ] maze solver
    - [ ] use depth first algorithm
    - [ ] map the whole labrinth
        - [ ] save to sd card?

- [ ] line following PID
    - [ ] setup motor drivers code
        - [ ] acceleration PID?
    - [ ] setup color detection sensors code

- [ ] merge every branch into a single one

- [ ] code and test the final task code (with all drivers finished)

## information/links to take into consideration
- [STM32 debugging](https://www.eggers-club.de/blog/2017/07/01/embedded-debugging-with-lldb-sure/)
- [Understand PID Control](https://www.mathworks.com/videos/understanding-pid-control-part-1-what-is-pid-control--1527089264373.html?s_tid=srchtitle_videos_main_7_PID%20Control)
- [Line Following PID](https://youtu.be/PP4fvBVe3rI?si=MxdATgHp2Bqobqsr)
- [Cascading PID](https://www.researchgate.net/profile/Muhammet-Biberoglu/publication/284888377_Tuning_Cascade_PID_Controllers_in_PMDC_Motor_Drives_A_Performance_Comparison_for_Different_Types_of_Tuning_Methods/links/5659b48108aeafc2aac4c729/Tuning-Cascade-PID-Controllers-in-PMDC-Motor-Drives-A-Performance-Comparison-for-Different-Types-of-Tuning-Methods.pdf)
- [Reference Motor PID](https://robotics.stackexchange.com/questions/21569/pid-controller-that-targets-a-set-rotation-and-a-set-velocity-at-that-rotation/21571#21571)

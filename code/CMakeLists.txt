cmake_minimum_required(VERSION 3.16)

# Possible values: openocd, pyocd, stlink, blackmagic. stlink is default
set(CMX_DEBUGGER "stlink")
# set(OPENOCD_CFG "${CMAKE_CURRENT_SOURCE_DIR}/openocd.cfg")

include(cubemx.cmake/cubemx.cmake)

# generate compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(testproject)

add_executable(mrcheese)
target_compile_definitions(mrcheese PRIVATE STM32F103xB)
cubemx_target(
    TARGET mrcheese
    IOC "${CMAKE_CURRENT_LIST_DIR}/mrcheese.ioc"
    STARTUP "${CMAKE_CURRENT_LIST_DIR}/startup_stm32f103xb.s"
)
target_include_directories(mrcheese PRIVATE
    "Middlewares/Third_Party/FreeRTOS/Source/include"
    "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3"
    "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2"
    "Middlewares/ST/STM32_USB_Device_Library/Core/Inc"
    "Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc"
    "USB_DEVICE/App"
    "USB_DEVICE/Target"
)
file(GLOB_RECURSE MIDDLEWARE_SRC
    "Middlewares/*.c"
)

file(GLOB_RECURSE USB_DEVICE_SRC
    "USB_DEVICE/*.c"
)
target_sources(mrcheese PRIVATE ${MIDDLEWARE_SRC} ${USB_DEVICE_SRC})

target_compile_options(mrcheese PRIVATE -Og -Wall -g -gdwarf-2)

# Depending on the project setup, sometimes one of these symbols must be omitted. (Cannot be reliably determined from the .ioc file)
target_compile_definitions(mrcheese PRIVATE USE_FULL_LL_DRIVER USE_HAL_DRIVER)

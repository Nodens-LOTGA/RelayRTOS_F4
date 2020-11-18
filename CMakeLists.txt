cmake_minimum_required(VERSION 3.15)

project(RelayRTOS_F4 LANGUAGES C CXX ASM)

option(FLASH_EXTERNAL_MEMORY "Flash external memory before flashing main application" OFF)

find_bsp(ID com.sysprogs.arm.stm32
  VERSION 2020.10
  MCU STM32F446RC
  CONFIGURATION "com.sysprogs.toolchainoptions.arm.libctype=--specs=nano.specs -u _printf_float -u _scanf_float"
  HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f446xx.xml)

register_imported_project(NAME RelayRTOS_F4 PATH RelayRTOS.gpdsc LEVEL 0)
add_bsp_based_executable(NAME RelayRTOS_F4
  SOURCES
    ${RelayRTOS_F4_LOCATION}/Core/Inc/FreeRTOSConfig.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/adc.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/bitmaps.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/colors.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/display.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/dma.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/ee24.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/ee24Config.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/fonts.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/gpio.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/i2c.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/interrupts.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/main.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/navigation.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/rtc.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/screens.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/settings.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/setupscreen.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/spi.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/stm32f4xx_hal_conf.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/stm32f4xx_it.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/tasks.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/tim.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/utils.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/w25qxx.h
    ${RelayRTOS_F4_LOCATION}/Core/Inc/w25qxxConf.h
    ${RelayRTOS_F4_LOCATION}/Core/Src/adc.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/aftererrscreen.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/bitmaps.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/colors.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/display.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/dma.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/ee24.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/errorscreen.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/fonts.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/freertos.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/gpio.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/i2c.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/interrupts.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/main.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/mainscreen.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/navigation.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/rtc.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/screens.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/screensaver.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/settings.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/setupscreen.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/spi.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/stm32f4xx_hal_msp.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/stm32f4xx_hal_timebase_tim.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/stm32f4xx_it.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/system_stm32f4xx.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/tasks.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/tim.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/utils.c
    ${RelayRTOS_F4_LOCATION}/Core/Src/w25qxx.c
    ${RelayRTOS_F4_LOCATION}/Drivers/CMSIS/DSP/Include/arm_common_tables.h
    ${RelayRTOS_F4_LOCATION}/Drivers/CMSIS/DSP/Include/arm_const_structs.h
    ${RelayRTOS_F4_LOCATION}/Drivers/CMSIS/DSP/Include/arm_math.h
    ${RelayRTOS_F4_LOCATION}/Drivers/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h
    ${RelayRTOS_F4_LOCATION}/Drivers/CMSIS/Device/ST/STM32F4xx/Include/system_stm32f4xx.h
    ${RelayRTOS_F4_LOCATION}/Drivers/CMSIS/Include/core_cm4.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy/stm32_hal_legacy.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_adc.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_adc_ex.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_cortex.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_def.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_dma.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_dma_ex.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_exti.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash_ex.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash_ramfunc.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_gpio.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_gpio_ex.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_i2c.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_i2c_ex.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_pwr.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_pwr_ex.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rcc.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rcc_ex.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rtc.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rtc_ex.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_spi.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim_ex.h
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/StackMacros.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/croutine.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/deprecated_definitions.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/event_groups.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/list.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/message_buffer.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/mpu_prototypes.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/mpu_wrappers.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/portable.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/projdefs.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/queue.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/semphr.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/stack_macros.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/stream_buffer.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/task.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include/timers.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/list.c
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/portmacro.h
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/queue.c
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/timers.c
  GENERATE_BIN
  GENERATE_MAP
  LINKER_SCRIPT
  STM32F446RC_flash.lds
  MEMORY_LIST_FILE
  Core/ExtraMemories/ExtraMemories)
target_compile_options(RelayRTOS_F4 PRIVATE $<IF:$<CONFIG:Release>,-O2,-Og>)
target_include_directories(RelayRTOS_F4
  PRIVATE
    ${RelayRTOS_F4_LOCATION}/Core/Inc
    ${RelayRTOS_F4_LOCATION}/Core/ExtraMemories
    ${RelayRTOS_F4_LOCATION}/Drivers/CMSIS/DSP/Include
    ${RelayRTOS_F4_LOCATION}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${RelayRTOS_F4_LOCATION}/Drivers/CMSIS/Include
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${RelayRTOS_F4_LOCATION}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/include
    ${RelayRTOS_F4_LOCATION}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
)

find_library(ARM_CORTEXM4LF_MATH_LIB arm_cortexM4lf_math ${RelayRTOS_F4_LOCATION}/Drivers/CMSIS/Lib/GCC)
target_link_libraries(RelayRTOS_F4 PRIVATE ${ARM_CORTEXM4LF_MATH_LIB} -lm)
target_compile_definitions(RelayRTOS_F4 
  PRIVATE
  flash_layout
  STM32F446xx
  USE_FREERTOS)

if(FLASH_EXTERNAL_MEMORY)
  target_compile_definitions(RelayRTOS_F4 PRIVATE FLASH_EXTERNAL_MEMORY)
endif()

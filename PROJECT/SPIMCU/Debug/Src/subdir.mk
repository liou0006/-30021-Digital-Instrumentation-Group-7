################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/30010_io.c \
../Src/adc.c \
../Src/flash.c \
../Src/interrupt.c \
../Src/joystick.c \
../Src/lcd.c \
../Src/led.c \
../Src/lsm9ds1.c \
../Src/main.c \
../Src/spiMaster.c \
../Src/spiSlave.c \
../Src/syscalls.c \
../Src/system_stm32f30x.c \
../Src/timer.c \
../Src/window.c 

OBJS += \
./Src/30010_io.o \
./Src/adc.o \
./Src/flash.o \
./Src/interrupt.o \
./Src/joystick.o \
./Src/lcd.o \
./Src/led.o \
./Src/lsm9ds1.o \
./Src/main.o \
./Src/spiMaster.o \
./Src/spiSlave.o \
./Src/syscalls.o \
./Src/system_stm32f30x.o \
./Src/timer.o \
./Src/window.o 

C_DEPS += \
./Src/30010_io.d \
./Src/adc.d \
./Src/flash.d \
./Src/interrupt.d \
./Src/joystick.d \
./Src/lcd.d \
./Src/led.d \
./Src/lsm9ds1.d \
./Src/main.d \
./Src/spiMaster.d \
./Src/spiSlave.d \
./Src/syscalls.d \
./Src/system_stm32f30x.d \
./Src/timer.d \
./Src/window.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F302R8Tx -DSTM32F3 -DNUCLEO_F302R8 -DDEBUG -DUSE_STDPERIPH_DRIVER=1 -c -I"C:/Users/liou-/OneDrive - Danmarks Tekniske Universitet/Elektrotekniologi - Master/1. semester/30021 Digital Instrumentation/-30021-Digital-Instrumentation-Group-7/PROJECT/SPIMCU/SPL/inc" -I"C:/Users/liou-/OneDrive - Danmarks Tekniske Universitet/Elektrotekniologi - Master/1. semester/30021 Digital Instrumentation/-30021-Digital-Instrumentation-Group-7/PROJECT/SPIMCU/Inc" -I"C:/Users/liou-/OneDrive - Danmarks Tekniske Universitet/Elektrotekniologi - Master/1. semester/30021 Digital Instrumentation/-30021-Digital-Instrumentation-Group-7/PROJECT/SPIMCU/cmsis" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/30010_io.cyclo ./Src/30010_io.d ./Src/30010_io.o ./Src/30010_io.su ./Src/adc.cyclo ./Src/adc.d ./Src/adc.o ./Src/adc.su ./Src/flash.cyclo ./Src/flash.d ./Src/flash.o ./Src/flash.su ./Src/interrupt.cyclo ./Src/interrupt.d ./Src/interrupt.o ./Src/interrupt.su ./Src/joystick.cyclo ./Src/joystick.d ./Src/joystick.o ./Src/joystick.su ./Src/lcd.cyclo ./Src/lcd.d ./Src/lcd.o ./Src/lcd.su ./Src/led.cyclo ./Src/led.d ./Src/led.o ./Src/led.su ./Src/lsm9ds1.cyclo ./Src/lsm9ds1.d ./Src/lsm9ds1.o ./Src/lsm9ds1.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/spiMaster.cyclo ./Src/spiMaster.d ./Src/spiMaster.o ./Src/spiMaster.su ./Src/spiSlave.cyclo ./Src/spiSlave.d ./Src/spiSlave.o ./Src/spiSlave.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/system_stm32f30x.cyclo ./Src/system_stm32f30x.d ./Src/system_stm32f30x.o ./Src/system_stm32f30x.su ./Src/timer.cyclo ./Src/timer.d ./Src/timer.o ./Src/timer.su ./Src/window.cyclo ./Src/window.d ./Src/window.o ./Src/window.su

.PHONY: clean-Src


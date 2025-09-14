################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/30010_io.c \
../Src/interrupt.c \
../Src/joystick.c \
../Src/led.c \
../Src/main.c \
../Src/syscalls.c \
../Src/system_stm32f30x.c 

OBJS += \
./Src/30010_io.o \
./Src/interrupt.o \
./Src/joystick.o \
./Src/led.o \
./Src/main.o \
./Src/syscalls.o \
./Src/system_stm32f30x.o 

C_DEPS += \
./Src/30010_io.d \
./Src/interrupt.d \
./Src/joystick.d \
./Src/led.d \
./Src/main.d \
./Src/syscalls.d \
./Src/system_stm32f30x.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F302R8Tx -DSTM32F3 -DNUCLEO_F302R8 -DDEBUG -DUSE_STDPERIPH_DRIVER=1 -c -I"C:/Users/liou-/OneDrive - Danmarks Tekniske Universitet/Elektrotekniologi - Master/1. semester/30021 Digital Instrumentation/-30021-Digital-Instrumentation-Group-7/project2/SPL/inc" -I"C:/Users/liou-/OneDrive - Danmarks Tekniske Universitet/Elektrotekniologi - Master/1. semester/30021 Digital Instrumentation/-30021-Digital-Instrumentation-Group-7/project2/Inc" -I"C:/Users/liou-/OneDrive - Danmarks Tekniske Universitet/Elektrotekniologi - Master/1. semester/30021 Digital Instrumentation/-30021-Digital-Instrumentation-Group-7/project2/cmsis" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/30010_io.cyclo ./Src/30010_io.d ./Src/30010_io.o ./Src/30010_io.su ./Src/interrupt.cyclo ./Src/interrupt.d ./Src/interrupt.o ./Src/interrupt.su ./Src/joystick.cyclo ./Src/joystick.d ./Src/joystick.o ./Src/joystick.su ./Src/led.cyclo ./Src/led.d ./Src/led.o ./Src/led.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/system_stm32f30x.cyclo ./Src/system_stm32f30x.d ./Src/system_stm32f30x.o ./Src/system_stm32f30x.su

.PHONY: clean-Src


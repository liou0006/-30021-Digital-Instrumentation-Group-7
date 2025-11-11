################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
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
../Src/main.c \
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
./Src/main.o \
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
./Src/main.d \
./Src/syscalls.d \
./Src/system_stm32f30x.d \
./Src/timer.d \
./Src/window.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F302R8Tx -DSTM32F3 -DNUCLEO_F302R8 -DDEBUG -DUSE_STDPERIPH_DRIVER=1 -c -I"C:/Users/s214680/OneDrive - Danmarks Tekniske Universitet/A Kandidat - Elektroteknologi/1. Term/30021 Digital Instrumentation/-30021-Digital-Instrumentation-Group-7/project2/SPL/inc" -I"C:/Users/s214680/OneDrive - Danmarks Tekniske Universitet/A Kandidat - Elektroteknologi/1. Term/30021 Digital Instrumentation/-30021-Digital-Instrumentation-Group-7/project2/Inc" -I"C:/Users/s214680/OneDrive - Danmarks Tekniske Universitet/A Kandidat - Elektroteknologi/1. Term/30021 Digital Instrumentation/-30021-Digital-Instrumentation-Group-7/project2/cmsis" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/30010_io.d ./Src/30010_io.o ./Src/adc.d ./Src/adc.o ./Src/flash.d ./Src/flash.o ./Src/interrupt.d ./Src/interrupt.o ./Src/joystick.d ./Src/joystick.o ./Src/lcd.d ./Src/lcd.o ./Src/led.d ./Src/led.o ./Src/main.d ./Src/main.o ./Src/syscalls.d ./Src/syscalls.o ./Src/system_stm32f30x.d ./Src/system_stm32f30x.o ./Src/timer.d ./Src/timer.o ./Src/window.d ./Src/window.o

.PHONY: clean-Src


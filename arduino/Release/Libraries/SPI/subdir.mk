################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/Users/Andrew/Dev/Arduino/Arduino.app/Contents/Resources/Java/hardware/arduino/avr/libraries/SPI/SPI.cpp 

CPP_DEPS += \
./Libraries/SPI/SPI.cpp.d 

LINK_OBJ += \
./Libraries/SPI/SPI.cpp.o 


# Each subdirectory must supply rules for building sources it contributes
Libraries/SPI/SPI.cpp.o: /Users/Andrew/Dev/Arduino/Arduino.app/Contents/Resources/Java/hardware/arduino/avr/libraries/SPI/SPI.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/Andrew/Dev/Arduino/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=161 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"/Users/Andrew/Dev/Arduino/Arduino.app/Contents/Resources/Java/hardware/arduino/avr/cores/arduino" -I"/Users/Andrew/Dev/Arduino/Arduino.app/Contents/Resources/Java/hardware/arduino/avr/variants/eightanaloginputs" -I"/Users/Andrew/Dev/Arduino/libraries/DHT-sensor-library" -I"/Users/Andrew/Dev/Arduino/libraries/Low-Power" -I"/Users/Andrew/Dev/Arduino/libraries/RF24" -I"/Users/Andrew/Dev/Arduino/Arduino.app/Contents/Resources/Java/hardware/arduino/avr/libraries/SPI" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '



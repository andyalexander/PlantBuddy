################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include arduino/core/subdir.mk
-include Libraries/SPI/subdir.mk
-include Libraries/RF24/subdir.mk
-include Libraries/Low-Power/subdir.mk
-include Libraries/DHT-sensor-library/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
/Users/Andrew/Dev/Arduino/PlantFriend/Release/arduino.ar \

ELF += \
PlantFriend.elf \

EEP += \
PlantFriend.eep \

FLASH_IMAGE += \
PlantFriend.hex \


# All Target
all: PlantFriend

# Tool invocations
PlantFriend: $(FLASH_IMAGE)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"/Users/Andrew/Dev/Arduino/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-size" -A "/Users/Andrew/Dev/Arduino/PlantFriend/Release/PlantFriend.elf"
	@echo 'Finished building target: $@'
	@echo ' '

/Users/Andrew/Dev/Arduino/PlantFriend/Release/arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"/Users/Andrew/Dev/Arduino/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-ar" rcs  "/Users/Andrew/Dev/Arduino/PlantFriend/Release/arduino.ar"   $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

PlantFriend.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"/Users/Andrew/Dev/Arduino/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-gcc" -w -Os -Wl,--gc-sections -mmcu=atmega328p  -o "/Users/Andrew/Dev/Arduino/PlantFriend/Release/PlantFriend.elf"   $(LINK_OBJ) $(AR)  "/Users/Andrew/Dev/Arduino/PlantFriend/Release/arduino.ar" "-L/Users/Andrew/Dev/Arduino/PlantFriend/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

PlantFriend.eep: $(ELF)
	@echo 'Create eeprom image'
	"/Users/Andrew/Dev/Arduino/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  "/Users/Andrew/Dev/Arduino/PlantFriend/Release/PlantFriend.elf" "/Users/Andrew/Dev/Arduino/PlantFriend/Release/PlantFriend.eep"
	@echo 'Finished building: $@'
	@echo ' '

PlantFriend.hex: $(EEP)
	@echo 'Create Flash image (ihex format)'
	"/Users/Andrew/Dev/Arduino/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-objcopy" -O ihex -R .eeprom  "/Users/Andrew/Dev/Arduino/PlantFriend/Release/PlantFriend.elf" "/Users/Andrew/Dev/Arduino/PlantFriend/Release/PlantFriend.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELF)$(C++_DEPS)$(AR)$(C_DEPS)$(CC_DEPS)$(FLASH_IMAGE)$(AR_OBJ)$(CPP_DEPS)$(LINK_OBJ)$(EEP)$(CXX_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) PlantFriend
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

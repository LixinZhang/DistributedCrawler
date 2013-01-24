################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../distributed/communication.cpp 

C_SRCS += \
../distributed/cJSON.c 

OBJS += \
./distributed/cJSON.o \
./distributed/communication.o 

C_DEPS += \
./distributed/cJSON.d 

CPP_DEPS += \
./distributed/communication.d 


# Each subdirectory must supply rules for building sources it contributes
distributed/%.o: ../distributed/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

distributed/%.o: ../distributed/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



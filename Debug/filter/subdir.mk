################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../filter/bloom.cpp \
../filter/hostFilter.cpp \
../filter/robotFilter.cpp \
../filter/suffixFilter.cpp 

OBJS += \
./filter/bloom.o \
./filter/hostFilter.o \
./filter/robotFilter.o \
./filter/suffixFilter.o 

CPP_DEPS += \
./filter/bloom.d \
./filter/hostFilter.d \
./filter/robotFilter.d \
./filter/suffixFilter.d 


# Each subdirectory must supply rules for building sources it contributes
filter/%.o: ../filter/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



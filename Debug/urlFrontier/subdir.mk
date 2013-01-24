################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../urlFrontier/minheap.cpp \
../urlFrontier/url.cpp \
../urlFrontier/urlfrontier.cpp 

OBJS += \
./urlFrontier/minheap.o \
./urlFrontier/url.o \
./urlFrontier/urlfrontier.o 

CPP_DEPS += \
./urlFrontier/minheap.d \
./urlFrontier/url.d \
./urlFrontier/urlfrontier.d 


# Each subdirectory must supply rules for building sources it contributes
urlFrontier/%.o: ../urlFrontier/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



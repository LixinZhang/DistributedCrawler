################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../consistentHash/CConHash.cpp \
../consistentHash/CMD5HashFun.cpp \
../consistentHash/CMd5.cpp \
../consistentHash/CNode_s.cpp \
../consistentHash/CRBTree.cpp \
../consistentHash/CVirtualNode_s.cpp 

OBJS += \
./consistentHash/CConHash.o \
./consistentHash/CMD5HashFun.o \
./consistentHash/CMd5.o \
./consistentHash/CNode_s.o \
./consistentHash/CRBTree.o \
./consistentHash/CVirtualNode_s.o 

CPP_DEPS += \
./consistentHash/CConHash.d \
./consistentHash/CMD5HashFun.d \
./consistentHash/CMd5.d \
./consistentHash/CNode_s.d \
./consistentHash/CRBTree.d \
./consistentHash/CVirtualNode_s.d 


# Each subdirectory must supply rules for building sources it contributes
consistentHash/%.o: ../consistentHash/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



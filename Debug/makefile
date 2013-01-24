################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include urlFrontier/subdir.mk
-include testCrawlerD/Debug/urlFrontier/subdir.mk
-include testCrawlerD/Debug/subdir.mk
-include testCrawlerD/Debug/filter/subdir.mk
-include testCrawlerD/Debug/fetch/subdir.mk
-include testCrawlerD/Debug/distributed/subdir.mk
-include testCrawlerD/Debug/consistentHash/subdir.mk
-include testCrawlerD/CrawlerD/urlFrontier/subdir.mk
-include testCrawlerD/CrawlerD/subdir.mk
-include testCrawlerD/CrawlerD/filter/subdir.mk
-include testCrawlerD/CrawlerD/fetch/subdir.mk
-include testCrawlerD/CrawlerD/distributed/subdir.mk
-include testCrawlerD/CrawlerD/consistentHash/subdir.mk
-include testCrawlerC/CrawlerC/urlFrontier/subdir.mk
-include testCrawlerC/CrawlerC/subdir.mk
-include testCrawlerC/CrawlerC/filter/subdir.mk
-include testCrawlerC/CrawlerC/fetch/subdir.mk
-include testCrawlerC/CrawlerC/distributed/subdir.mk
-include testCrawlerC/CrawlerC/consistentHash/subdir.mk
-include testCrawlerB/Debug/urlFrontier/subdir.mk
-include testCrawlerB/Debug/subdir.mk
-include testCrawlerB/Debug/filter/subdir.mk
-include testCrawlerB/Debug/fetch/subdir.mk
-include testCrawlerB/Debug/distributed/subdir.mk
-include testCrawlerB/Debug/consistentHash/subdir.mk
-include testCrawlerB/CrawlerB/urlFrontier/subdir.mk
-include testCrawlerB/CrawlerB/subdir.mk
-include testCrawlerB/CrawlerB/filter/subdir.mk
-include testCrawlerB/CrawlerB/fetch/subdir.mk
-include testCrawlerB/CrawlerB/distributed/subdir.mk
-include testCrawlerB/CrawlerB/consistentHash/subdir.mk
-include testCrawlerA/Debug/urlFrontier/subdir.mk
-include testCrawlerA/Debug/subdir.mk
-include testCrawlerA/Debug/filter/subdir.mk
-include testCrawlerA/Debug/fetch/subdir.mk
-include testCrawlerA/Debug/distributed/subdir.mk
-include testCrawlerA/Debug/consistentHash/subdir.mk
-include testCrawlerA/CrawlerA/urlFrontier/subdir.mk
-include testCrawlerA/CrawlerA/subdir.mk
-include testCrawlerA/CrawlerA/filter/subdir.mk
-include testCrawlerA/CrawlerA/fetch/subdir.mk
-include testCrawlerA/CrawlerA/distributed/subdir.mk
-include testCrawlerA/CrawlerA/consistentHash/subdir.mk
-include filter/subdir.mk
-include fetch/subdir.mk
-include distributed/subdir.mk
-include consistentHash/subdir.mk
-include Debug_MachineA/urlFrontier/subdir.mk
-include Debug_MachineA/subdir.mk
-include Debug_MachineA/filter/subdir.mk
-include Debug_MachineA/fetch/subdir.mk
-include Debug_MachineA/distributed/subdir.mk
-include Debug_MachineA/consistentHash/subdir.mk
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

# All Target
all: WebCrawler

# Tool invocations
WebCrawler: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -Lcrypto -o"WebCrawler" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) WebCrawler
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

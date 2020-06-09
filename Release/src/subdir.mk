################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/connect.c \
../src/listint.c \
../src/main.c \
../src/mapint.c \
../src/message_handler.c \
../src/worker.c 

OBJS += \
./src/connect.o \
./src/listint.o \
./src/main.o \
./src/mapint.o \
./src/message_handler.o \
./src/worker.o 

C_DEPS += \
./src/connect.d \
./src/listint.d \
./src/main.d \
./src/mapint.d \
./src/message_handler.d \
./src/worker.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo '正在构建文件： $<'
	@echo '正在调用： GNU C 编译器'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '



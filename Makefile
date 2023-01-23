PROJECT_PATH = $(PWD)

INLUDE_PATH:=$(PROJECT_PATH) $(PROJECT_PATH)/include

C_FILE_C = $(PROJECT_PATH)/*.c
C_FILE = $(wildcard $(C_FILE_C))
OBJS = $(C_FILE:.c=.o)

FILE_PATH = $(PROJECT_PATH)/../file-5.44/build/lib
HS_PATH = $(FILE_PATH)/../hyperscan/lib

INLUDE_PATH+=$(HS_PATH)/../src $(FILE_PATH)/../src

C_FLAGS := -O0 -g

PRE_DEF := -DPCRE2_CODE_UNIT_WIDTH=8

LIB_IN :=  -lmagic -lpcre2-8 -lhs -Wl,-rpath=$(HS_PATH) 
LIB_L := $(FILE_PATH) $(HS_PATH) 
LIB_FLAGS:=$(patsubst %,-L%,$(LIB_L))
I_FLAGS:=$(patsubst %,-I%,$(INLUDE_PATH))

I_FLAGS += $(PRE_DEF)

TARGET := interfile

GCC:= gcc

%.o:%.c
	$(GCC) $(I_FLAGS) -c $^

all:$(OBJS)
	$(GCC) $(LIB_FLAGS) $(C_FLAGS) $^ $(LIB_IN) -o $(TARGET)

clean:
	rm -rf *.o $(TARGET)

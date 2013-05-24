SRC  = usb_com.c

TARGET = usb_com

all: $(TARGET)

$(TARGET): 
	gcc -Wall $(SRC) -o $(TARGET) -lusb

ifdef SystemRoot
   RM = del /S /Q
else
   ifeq ($(shell uname), Linux)
      RM = shopt -s globstar; rm -rf 
   endif
endif

clean:
	$(RM) $(TARGET)
	

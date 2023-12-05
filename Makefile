CX = g++
CXFLAGS = -g -Wall

CV = `pkg-config opencv4 --cflags --libs`
DXLFLAGS = -I/usr/local/include/dynamixel_sdk
DXLFLAGS += -ldxl_x64_cpp
DXLFLAGS += -lrt


TARGET = line
OBJS = main.o dxl.o linetracer.o
$(TARGET) : $(OBJS)
	$(CX) $(CXFLAGS) -o $(TARGET) $(OBJS) $(DXLFLAGS) $(CV)
main.o : main.cpp dxl.hpp
	$(CX) $(CXFLAGS) -c main.cpp $(DXLFLAGS) $(CV)
dxl.o : dxl.hpp dxl.cpp
	$(CX) $(CXFLAGS) -c dxl.cpp $(DXLFLAGS) 
linetracer.o : linetracer.cpp linetracer.hpp
	$(CX) $(CXFLAGS) -c linetracer.cpp $(DXLFLAGS) $(CV)

.PHONY: all
all: $(TARGET)
clean:
	rm -rf $(TARGET) $(OBJS)

DEPEND_FILE = depend_file
BINDIR = /usr/local/bin
DATADIR = /usr/local/share/waroid

CC = gcc
CXX = g++
CFLAGS = 
CXXFLAGS = -D__RPI__

INCPATH = 
LINK = g++
LIBS = -lm -lpthread -lwiringPi -lasound -lsndfile

# FILE
CSOURCES = 

CPPSOURCES = main.cpp \
	common/GRCSoundWorker.cpp \
	common/GRCWave.cpp \
	communication/GRCAcceptSession.cpp \
	communication/GRCBaseSession.cpp \
	communication/GRCConnectSession.cpp \
	communication/GRCSerialSession.cpp \
	communication/GRCSockAddr.cpp \
	communication/GRCTcpSession.cpp \
	core/GRCBuffer.cpp \
	core/GRCCoreUtil.cpp \
	core/GRCJsonData.cpp \
	core/GRCLogger.cpp \
	core/GRCObject.cpp \
	core/GRCString.cpp \
	Defines.cpp \
	ControlBoardSession.cpp \
	RobotData.cpp \
	RobotInfo.cpp \
	UserSession.cpp \
	Manager.cpp

OBJECTS = $(CSOURCES:%.c=%.o)
OBJECTS += $(CPPSOURCES:%.cpp=%.o)

SOUNDS = data/sound/Common_Booting_Sound.wav \
	data/sound/GATLING.wav \
	data/sound/CANNON.wav \

	
JSONS = data/json/RobotData.json

TARGET = WaroidSingle

# Implict rules
.SUFFIXES : .cpp .c

.cpp.o: $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
.c.o: $(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

.PHONY: all clean depend install uninstall kill

all : $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK) -o $@ $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEPEND_FILE)

depend:
	#$(CC) -MM $(CSOURCES) > $(DEPEND_FILE)
	$(CXX) -MM $(CPPSOURCES) > $(DEPEND_FILE)

rebuild: clean depend all

install:
	sudo mkdir -p $(DATADIR)
	sudo cp -f $(TARGET) $(BINDIR)
	sudo cp -f $(SOUNDS) $(DATADIR)
	sudo cp -f $(JSONS) $(DATADIR)

uninstall:
	- sudo killall $(TARGET)
	sudo rm -fr $(DATADIR)
	sudo rm -f $(BINDIR)/$(TARGET)

kill:
	- sudo killall $(TARGET)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(MAKECMDGOALS), install)
ifneq ($(MAKECMDGOALS), uninstall)
ifneq ($(CSOURCES), )
ifneq ($(CPPSOURCE), )
-include $(DEPEND_FILE)
endif
endif
endif
endif
endif
endif

COMPILE_FLAGS := $(shell wx-config --cxxflags) -g -c
CXX_FLAGS := -std=c++11
LINK_FLAGS := $(shell wx-config --libs)

AFFL : AFFL_GUIApp.o AFFL_GUIMain.o lists.o md5.o
	g++  -o AFFL AFFL_GUIApp.o AFFL_GUIMain.o lists.o md5.o $(LINK_FLAGS)
AFFL_GUIApp.o : src/AFFL_GUIApp.cpp include/AFFL_GUIApp.h
	g++ $(CXX_FLAGS) $(COMPILE_FLAGS) -Iinclude src/AFFL_GUIApp.cpp
AFFL_GUIMain.o : src/AFFL_GUIMain.cpp include/AFFL_GUIMain.h
	g++ $(CXX_FLAGS) $(COMPILE_FLAGS) -Iinclude src/AFFL_GUIMain.cpp
md5.o : src/md5.c include/md5.h
	gcc $(COMPILE_FLAGS) -Iinclude src/md5.c
lists.o : src/lists.cpp include/lists.h
	g++ $(CXX_FLAGS) $(COMPILE_FLAGS) -Iinclude src/lists.cpp
	
clean:
	rm *.o
	rm AFFL
#.SILENT:

ifneq ($(WINDIR),)
  ARCH=WIN32
  GBUILDSTR=WIN32-i686-vc10
else
  UNAME=$(shell uname)
  ifeq ($(UNAME), Linux)
    ARCH=linux
  else ifeq ($(UNAME), Darwin)
    ARCH=OSX
  else
    $(error The Makefile does not recognize the architecture: $(UNAME))
  endif
endif


all: gen debug release

gen:
	mkdir -p ./build
    ifeq ($(ARCH), linux)
	  mkdir -p ./build/Release
	  mkdir -p ./build/Debug	  
	  cd ./build/Release; cmake -DCMAKE_BUILD_TYPE=Release ../../
	  cd ./build/Debug; cmake -DCMAKE_BUILD_TYPE=Debug ../../
    else ifeq ($(ARCH), WIN32)
	  cd ./build; cmake -DGBUILDSTR=$(GBUILDSTR) -DCMAKE_INSTALL_PREFIX=$(G)/src/MinVR/install ../ -G "Visual Studio 10 Win64"
    else ifeq ($(ARCH), OSX)
	  cd ./build; cmake ../ -G Xcode
    endif

debug:
    ifeq ($(ARCH), linux) 
	  cd ./build/$(GBUILDSTR)/Debug; make
    else
	  @echo "Open the project file to build the project on this architecture."
    endif

release opt:
    ifeq ($(ARCH), linux) 
	  cd ./build/$(GBUILDSTR)/Release; make
    else
	  @echo "Open the project file to build the project on this architecture."
    endif

install:
    ifeq ($(ARCH), linux) 
	  cd ./build/$(GBUILDSTR)/Debug; make install
	  cd ./build/$(GBUILDSTR)/Release; make install
    else
	  @echo "Open the project file to run make install on this architecture."
    endif
	
clean:
    ifeq ($(ARCH), linux) 
	  cd ./build/$(GBUILDSTR)/Debug; make clean
	  cd ./build/$(GBUILDSTR)/Release; make clean
    else
	  @echo "Open the project file to run make clean on this architecture."
    endif
	  
clobber:
	rm -rf ./build
	rm -rf ./dependencies/boost
	rm -rf ./dependencies/glfw
	rm -rf ./install

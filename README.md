# CLI Media Browser and Player Application

## Overview
This project is a command-line interface (CLI) application for Linux that serves as a basic media (audio and video) browser and player with advanced features. It supports browsing media files, managing playlists, editing metadata, and playing music. Additionally, it integrates with the S32K144 board for external device control.

---

## Features

### **[Basic Features]**

#### **1. Application Behavior:**
1. The application can run:
   - From any directory on your computer.
   - From an external USB device (requires mounting the USB and parsing files).

#### **2. Available Options:**

1. **Media File Browsing:**
   - View a list of media files (audio and video) in the current folder and all subfolders.
   - Includes pagination: Display 25 files per page.

2. **Playlist Management:**
   - List existing playlists.
   - View a specific playlist.
   - Create, update, or delete playlists.

3. **Metadata Handling (using Taglib):**
   - Accessible from either the media list or a playlist.
   - Display metadata of a media file:
     - **Audio:** Track name, Album, Artist, Duration, Genre, Publisher, Publish Year, etc.
     - **Video:** Name, Size, Duration, Bitrate, Codec, etc.
   - Edit metadata:
     - Modify existing values.
     - Add new metadata keys.

4. **Music Playback (using SDL2 Library):**
   - Supports basic controls:
     - Play/Pause.
     - Next/Previous.
   - Automatically moves to the next song when the current song finishes.
   - Displays the current playback time and total duration.

5. **Device Volume Control:**
   - Change the volume of the device directly from the application.

---

### **[External Device Connection]**

#### **1. Integration with S32K144 Board:**
- Volume control via ADC on the board.
- Playback control using buttons:
  - Play, pause, stop, next, and previous music.
- Display the currently playing song’s information on the board’s screen.

#### **2. Deliverables:**

1. **Software Requirement Specification:**
   - Use case descriptions.
   - Detailed workflows.

2. **Design Document:**
   - **Architecture Design:**
     - Implementation of the MVC pattern.
   - **Detailed Design:**
     - Modules and component-level descriptions.

3. **Code & Unit Tests:**
   - Implementation of the core application.
   - Unit tests for individual components.

---
### **How to Install Library FTXUI**

This guide will walk you through the steps to set up the FTXUI library in your project. FTXUI is a library for creating interactive text-based user interfaces in C++.

#### 1. Install FTXUI Library
FTXUI is a C++ library for user interfaces. To use it, you need to install the library. Follow the steps below if it is not already installed:

##### a. Use Git to Clone the Repository

```bash
git clone https://github.com/ArthurSonzogni/FTXUI.git
```

##### b. Build and Install the Library

```bash
cd FTXUI
mkdir build
cd build
cmake ..
make
sudo make install
```

This process will install FTXUI on your system.

#### 2. Configure Your Project to Use FTXUI

You need to update your project's build system to include FTXUI.

##### For Makefile

Add the following configurations to your `Makefile`:

```makefile
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LIBS = -lftxui-dom -lftxui-component -lftxui-screen

# Source and output files
SRC = src/controller/ManagerController.cpp
OBJ = $(SRC:%.cpp=build/%.o)

# Build targets
all: app

app: $(OBJ)
	$(CXX) $(OBJ) -o app $(LDFLAGS) $(LIBS)

build/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build app
```

##### For CMake

If you are using `CMake`, update your `CMakeLists.txt` file:

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# Include FTXUI
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Add executable
add_executable(app src/controller/ManagerController.cpp)

# Link FTXUI libraries
target_link_libraries(app ftxui-dom ftxui-component ftxui-screen)
```

---

### **How to Install Library TagLib**

TagLib is a library for reading and editing metadata of audio files. Follow the steps below to install it:

#### 1. Install TagLib Using Package Manager

```bash
sudo apt update
sudo apt install libtag1v5 libtag1v5-dev
```

This will install the TagLib library and its development headers.

#### 2. Configure Your Project to Use TagLib

Update your build system to include TagLib.

##### For Makefile

Add the following to your `Makefile`:

```makefile
CXXFLAGS += -ltag
```

##### For CMake

Update your `CMakeLists.txt` file:

```cmake
target_link_libraries(app tag)
```

#### 3. Verify Installation

Compile your project and ensure TagLib is linked correctly. Use `#include <taglib/fileref.h>` in your code to test it.

---

### **How to Install Library SDL2**

SDL2 is a cross-platform library for multimedia applications. Follow these steps to install it:

#### 1. Install SDL2 Using Package Manager

```bash
sudo apt update
sudo apt install libsdl2-2.0-0 libsdl2-dev
```

If you need additional SDL2 modules, such as for image, mixer, or TTF support, install them as follows:

```bash
sudo apt install libsdl2-image-2.0-0 libsdl2-image-dev
sudo apt install libsdl2-mixer-2.0-0 libsdl2-mixer-dev
sudo apt install libsdl2-ttf-2.0-0 libsdl2-ttf-dev
```

#### 2. Configure Your Project to Use SDL2

Update your build system to include SDL2.

##### For Makefile

Add the following to your `Makefile`:

```makefile
CXXFLAGS += -lSDL2
```

##### For CMake

Update your `CMakeLists.txt` file:

```cmake
target_link_libraries(app SDL2)
```

#### 3. Verify Installation

Compile your project and ensure SDL2 is linked correctly. Use `#include <SDL2/SDL.h>` in your code to test it.

---

#### 3. Verify Installation

Compile your project:

```bash
make  # If using Makefile
```

or

```bash
cmake . && make  # If using CMake
```

Run the resulting executable:

```bash
./app
```

If everything is set up correctly, your program will successfully include and use the libraries.

---

### Troubleshooting

1. **File Not Found Error:**
   - Ensure the library files are installed in `/usr/local/include` and `/usr/local/lib`.
   - Verify the `CXXFLAGS` and `LDFLAGS` paths in your Makefile or CMakeLists.txt.

2. **Permission Denied:**
   - Run `sudo make install` when installing the library.

3. **Library Linking Issues:**
   - Make sure the `LIBS` or `target_link_libraries` line includes all required libraries: `ftxui-dom`, `ftxui-component`, `ftxui-screen`, `tag`, and `SDL2`.

---

You are now ready to use FTXUI, TagLib, and SDL2 to build interactive and multimedia-enabled CLI applications.

---
- [Github Mock-project](https://github.com/thanhetebkdn/MOCK-project)
- [Notion Mock-project](https://thanhdeptrai.notion.site/MOCK-project-170f363afe3980b6bc66f2a9bb8b923d?pvs=4)


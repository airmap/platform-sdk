## How to Compile Platform SDK on MacOS and Android x86 

### Development Environment

- macOS Catalina version 10.15.4
- Apple clang version 11.0.3 (installed with Xcode 11.4.1)
- Homebrew 2.4.0
- Qt (5.11.3 or newer)
- ndk (r16b or newer)

### Mac compile method
1. update submodule by **git submodule update --init --recursive**
2. First run tools/osx/setup.sh command to install dependency tools (We not use the brew install Qt version);
4. Create an separate directory and run the following command

```
mkdir build-mac
cd build-mac
cmake .. -DAIRMAP_ENABLE_QT=ON -DQt5_DIR="/Applications/Qt/5.11.3/clang_64/lib/cmake/Qt5"
make
```
The **DQt5_DIR** is depenency on the Qt version installed. You need to change to the Qt install path of your computer.
The output binary DataPilot will output at path **build-mac/src/airmap/qt/**

### Android x86 compile method
1. update submodule by **git submodule update --init --recursive**
2. install ndk version r16b or newer
3. Create an separate directory and run the following command

```
mkdir build-mac
cd build-mac
cmake ..  -DCMAKE_SYSTEM_NAME=Android -DANDROID_PLATFORM=android-16 -DANDROID_ABI=x86 -DANDROID_NDK=/Users/tbago/Library/Android/android-ndk-r16b -DANDROID_STL=c++_static -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/Users/tbago/Library/Android/android-ndk-r16b/build/cmake/android.toolchain.cmake -DANDROID_TOOLCHAIN=clang -DANDROID=1 -DAIRMAP_ENABLE_QT=ON -DQt5_DIR="/Applications/Qt/5.11.3/android_x86/lib/cmake/Qt5" -DQt5Core_DIR="/Applications/Qt/5.11.3/android_x86/lib/cmake/Qt5Core"
make
```
The ndk and Qt path is dependeny by your install. Set the ndk path and Qt path by youself.

4. The boost library cannot build by the platform sdk cmake. So you need to clone the [Boost Library For Android](https://github.com/moritz-wundke/Boost-for-Android). And run the following command to build boost library:
```
./build-android.sh /Users/tbago/Library/Android/android-ndk-r19c --boost="1.70.0" --with-libraries=date_time,filesystem,log,program_options,system,test,thread --arch=x86 --target-version=16
```
This github library only  support boost version 1.70.0 and 1.73.0. So the boost library is not the same library as MacOS. When compile complate. You need copy the boost header file from **Boost-for-Android/build/out/x86/include/boost-1_70** to **airmap-platform-sdk/build-android/external/include** And copy the boost library **Boost-for-Android/build/out/x86/lib/*.a** to **airmap-platform-sdk/build-android/external/lib** and then rename the library path as following:
```
    libboost_system.a
    libboost_atomic.a
    libboost_test_exec_monitor.a
    libboost_chrono.a
    libboost_thread.a
    libboost_date_time.a
    libboost_unit_test_framework.a
    libboost_filesystem.a
    libboost_log.a       
    libboost_log_setup.a    
    libboost_prg_exec_monitor.a
    libboost_program_options.a
    libboost_regex.a    
```
And then you have to manual change the **airmapd.cmake**
```
find_package(Boost 1.70.0 QUIET REQUIRED date_time filesystem log program_options system thread)
```
And then run the cmake and make command again. The output file will in the following path: **airmap-platform-sdk/build-android/src/airmap/qt/libairmap-qt.so**


### More Info
1. I update the boost version to 1.72.0, because the boost version 1.70.0 cannot compile with the following error:
```
clang: error: unknown argument: '-fcoalesce-templates'
```
2. Sometimes the clang cannot compile boost library, you need to try to recompile again.

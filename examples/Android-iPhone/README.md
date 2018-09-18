# Android and iOS from Android Studio
This is an example for how to build both Android and iOS from the same gradle powered Android Studio project

## Prerequisites

1. Install Android Studio
  * Download from [Android Studio](https://developer.android.com/studio/)
  * Install the ndk ![Package Manager](/examples/Android-iPhone/installNDK.gif)

## Build Android

1. if *only* building android edit [settings.gradle](/examples/Android-iPhone/app/settings.gradle) to read: `include ':app'`
2. Build app as normal

## Build iOS

1. Install CMake to system if building iOS
  *
  Method | Steps
  --- | :---
  HomeBrew | <ul><li>[Install brew](https://brew.sh/)</li><li>brew install cmake</li></ul>
  Source | [install cmake](https://cmake.org/install/)
  installer | [cmake prebuilt](https://cmake.org/download/)
2. If building XCode, edit [XCode/build.gradle](/examples/Android-iPhone/XCode/build.gradle#L26-L34) Set the username password for the AppStore and fill in the signing information
3. Install XCode from the AppStore
4. Add your signing information to CMake [XCode/CMakeLists.txt](/examples/Android-iPhone/XCode/CMakeLists.txt#L29)
5. run the gradle task :XCode:assemble
6. to deploy to the AppStore run gradle tasks
   * :XCode:archive
   * :XCode:package
   * :XCode:appstoreUpload
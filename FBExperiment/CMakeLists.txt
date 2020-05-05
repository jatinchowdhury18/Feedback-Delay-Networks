cmake_minimum_required(VERSION 3.4)

project("FBExperiment")

# FRUT stuff
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/FRUT/prefix/FRUT/cmake")
include(Reprojucer)

# Comment this line to build without the reverb tester
add_definitions(-DBUILD_WITH_REVERB_TESTER)

jucer_project_begin(
    JUCER_VERSION "5.4.7"
    PROJECT_FILE "${FBExperiment_jucer_FILE}"
    PROJECT_ID "hxzNW"
)

jucer_project_settings(
    PROJECT_NAME "FBExperiment"
    PROJECT_VERSION "1.0.0"
    COMPANY_NAME "Kim and Jatin"
    COMPANY_COPYRIGHT "Kim and Jatin"
    REPORT_JUCE_APP_USAGE OFF
    DISPLAY_THE_JUCE_SPLASH_SCREEN OFF
    PROJECT_TYPE "Audio Plug-in"
    BUNDLE_IDENTIFIER "com.kimandjatin.FBExperiment"
    CXX_LANGUAGE_STANDARD "C++17"
    HEADER_SEARCH_PATHS
        ${CMAKE_CURRENT_LIST_DIR}/../shared/
)

jucer_audio_plugin_settings(
    PLUGIN_FORMATS
        VST3
        AU
        Standalone
    PLUGIN_NAME "FBExperiment"
    PLUGIN_DESCRIPTION "FBExperiment"
    PLUGIN_MANUFACTURER "Kim and Jatin"
    PLUGIN_MANUFACTURER_CODE "KAJ"
    PLUGIN_CODE "JBvj"
    PLUGIN_AU_EXPORT_PREFIX "FBExperimentAU"
)

# add shared files
add_subdirectory(../shared shared_build)

get_directory_property(MYDEFS COMPILE_DEFINITIONS)
if(MYDEFS MATCHES "^BUILD_WITH_REVERB_TESTER")
    MESSAGE(STATUS "Building with ReverbTester")
    jucer_project_files("FBExperiment/ReverbTester"
    # Compile   Xcode     Binary    File
    #           Resource  Resource
      .         .         .         "${CMAKE_CURRENT_LIST_DIR}/../ReverbTester/Source/Console.h"
      .         .         .         "${CMAKE_CURRENT_LIST_DIR}/../ReverbTester/Source/FilePlayer.h"
      .         .         .         "${CMAKE_CURRENT_LIST_DIR}/../ReverbTester/Source/IRViewer.h"
      .         .         .         "${CMAKE_CURRENT_LIST_DIR}/../ReverbTester/Source/ReverbTesterEditor.h"
      .         .         .         "${CMAKE_CURRENT_LIST_DIR}/../ReverbTester/Source/ReverbTesterProcessor.h"
      x         .         .         "${CMAKE_CURRENT_LIST_DIR}/../ReverbTester/Source/Console.cpp"
      x         .         .         "${CMAKE_CURRENT_LIST_DIR}/../ReverbTester/Source/FilePlayer.cpp"
      x         .         .         "${CMAKE_CURRENT_LIST_DIR}/../ReverbTester/Source/IRViewer.cpp"
      x         .         .         "${CMAKE_CURRENT_LIST_DIR}/../ReverbTester/Source/ReverbTesterEditor.cpp"
      x         .         .         "${CMAKE_CURRENT_LIST_DIR}/../ReverbTester/Source/ReverbTesterProcessor.cpp"
)
else()
    MESSAGE(STATUS "Building without ReverbTester")
endif()

jucer_project_files("FBExperiment/Source"
# Compile   Xcode     Binary    File
#           Resource  Resource
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/FBExperimentProcessor.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/FBProc.h"
  x         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/FBExperimentProcessor.cpp"
)

# JUCE modules
jucer_project_module(
    juce_audio_basics
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    juce_audio_devices
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    juce_audio_plugin_client
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
JUCE_VST3_CAN_REPLACE_VST2 OFF
)

jucer_project_module(
    juce_audio_formats
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    juce_audio_processors
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    juce_audio_utils
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    juce_core
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    juce_data_structures
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    juce_dsp
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    juce_events
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    juce_graphics
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    juce_gui_basics
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    juce_gui_extra
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules"
)

jucer_project_module(
    foleys_gui_magic
    PATH "$\{CMAKE_CURRENT_LIST_DIR\}/../modules"
    FOLEYS_SHOW_GUI_EDITOR_PALLETTE OFF # Turn on to edit your GUI!
)

jucer_export_target(
    "Xcode (MacOSX)"
    EXTERNAL_LIBRARIES_TO_LINK FDNShared
)

jucer_export_target_configuration(
    "Xcode (MacOSX)"
    NAME "Debug"
    DEBUG_MODE ON
    BINARY_NAME "FBExperiment"
)

jucer_export_target_configuration(
    "Xcode (MacOSX)"
    NAME "Release"
    DEBUG_MODE OFF
    BINARY_NAME "FBExperiment"
)

jucer_export_target(
    "Visual Studio 2017"
    EXTRA_PREPROCESSOR_DEFINITIONS
        _CRT_SECURE_NO_WARNINGS
    EXTERNAL_LIBRARIES_TO_LINK FDNShared
)

jucer_export_target_configuration(
    "Visual Studio 2017"
    NAME "Debug"
    DEBUG_MODE ON
    BINARY_NAME "FBExperiment"
)

jucer_export_target_configuration(
    "Visual Studio 2017"
    NAME "Release"
    DEBUG_MODE OFF
    BINARY_NAME "FBExperiment"
)

jucer_export_target(
    "Linux Makefile"
    EXTERNAL_LIBRARIES_TO_LINK FDNShared
)

jucer_export_target_configuration(
    "Linux Makefile"
    NAME "Debug"
    DEBUG_MODE ON
    BINARY_NAME "FBExperiment"
)

jucer_export_target_configuration(
    "Linux Makefile"
    NAME "Release"
    DEBUG_MODE OFF
    BINARY_NAME "FBExperiment"
)

jucer_project_end()

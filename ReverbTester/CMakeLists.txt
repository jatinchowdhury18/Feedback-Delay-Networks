cmake_minimum_required(VERSION 3.4)

project("ReverbTester")

# FRUT stuff
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/FRUT/prefix/FRUT/cmake")
include(Reprojucer)

jucer_project_begin(
    JUCER_VERSION "5.4.7"
    PROJECT_FILE "${ReverbTester_jucer_FILE}"
    PROJECT_ID "Bkshq"
)

jucer_project_settings(
    PROJECT_NAME "ReverbTester"
    PROJECT_VERSION "1.0.0"
    COMPANY_NAME "Kim and Jatin"
    COMPANY_COPYRIGHT "Kim and Jatin"
    REPORT_JUCE_APP_USAGE OFF
    DISPLAY_THE_JUCE_SPLASH_SCREEN OFF
    PROJECT_TYPE "Audio Plug-in"
    BUNDLE_IDENTIFIER "com.kimandjatin.ReverbTester"
    CXX_LANGUAGE_STANDARD "C++17"
    HEADER_SEARCH_PATHS
        ${CMAKE_CURRENT_LIST_DIR}/../shared/
)

jucer_audio_plugin_settings(
    PLUGIN_FORMATS
        VST3
        AU
        Standalone
    PLUGIN_NAME "ReverbTester"
    PLUGIN_DESCRIPTION "ReverbTester"
    PLUGIN_MANUFACTURER "Kim and Jatin"
    PLUGIN_MANUFACTURER_CODE "KAJ"
    PLUGIN_CODE "ObSJ"
    PLUGIN_AU_EXPORT_PREFIX "ReverbTesterAU"
)

# Source files
jucer_project_files("ReverbTester/shared"
# Compile   Xcode     Binary    File
#           Resource  Resource
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/../shared/PluginProcessor.h"
)

jucer_project_files("ReverbTester/Source"
# Compile   Xcode     Binary    File
#           Resource  Resource
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/ReverbTesterProcessor.h"
  x         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/ReverbTesterProcessor.cpp"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/ReverbTesterEditor.h"
  x         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/ReverbTesterEditor.cpp"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/Console.h"
  x         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/Console.cpp"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/IRViewer.h"
  x         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/IRViewer.cpp"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/FilePlayer.h"
  x         .         .         "${CMAKE_CURRENT_LIST_DIR}/Source/FilePlayer.cpp"
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
    PATH "${CMAKE_CURRENT_LIST_DIR}/../modules"
    FOLEYS_SHOW_GUI_EDITOR_PALLETTE OFF # Turn on to edit your GUI!
)

jucer_export_target(
    "Xcode (MacOSX)"
)

jucer_export_target_configuration(
    "Xcode (MacOSX)"
    NAME "Debug"
    DEBUG_MODE ON
    BINARY_NAME "ReverbTester"
)

jucer_export_target_configuration(
    "Xcode (MacOSX)"
    NAME "Release"
    DEBUG_MODE OFF
    BINARY_NAME "ReverbTester"
)

jucer_export_target(
    "Visual Studio 2017"
    EXTRA_PREPROCESSOR_DEFINITIONS
        _CRT_SECURE_NO_WARNINGS
)

jucer_export_target_configuration(
    "Visual Studio 2017"
    NAME "Debug"
    DEBUG_MODE ON
    BINARY_NAME "ReverbTester"
)

jucer_export_target_configuration(
    "Visual Studio 2017"
    NAME "Release"
    DEBUG_MODE OFF
    BINARY_NAME "ReverbTester"
)

jucer_export_target(
    "Linux Makefile"
)

jucer_export_target_configuration(
    "Linux Makefile"
    NAME "Debug"
    DEBUG_MODE ON
    BINARY_NAME "ReverbTester"
)

jucer_export_target_configuration(
    "Linux Makefile"
    NAME "Release"
    DEBUG_MODE OFF
    BINARY_NAME "ReverbTester"
)

jucer_project_end()

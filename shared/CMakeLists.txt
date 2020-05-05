cmake_minimum_required(VERSION 3.4)

project("FDNShared")

# FRUT stuff
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules/FRUT/prefix/FRUT/cmake")
include(Reprojucer)

jucer_project_begin(
    JUCER_VERSION "5.4.7"
    PROJECT_FILE "${FDNShared_jucer_FILE}"
    PROJECT_ID "aSweR"
)

jucer_project_settings(
    PROJECT_NAME "FDNShared"
    PROJECT_VERSION "1.0.0"
    COMPANY_NAME "Kim and Jatin"
    COMPANY_COPYRIGHT "Kim and Jatin"
    REPORT_JUCE_APP_USAGE OFF
    DISPLAY_THE_JUCE_SPLASH_SCREEN OFF
    PROJECT_TYPE "Static Library"
    BUNDLE_IDENTIFIER "com.kimandjatin.FDNShared"
    CXX_LANGUAGE_STANDARD "C++17"
)

jucer_project_files("shared"
# Compile   Xcode     Binary    File
#           Resource  Resource
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/Biquad.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/DCBlocker.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/DelayLine.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/DelayUtils.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/DryWetProcessor.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/FDN.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/LPF.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/MixingMatrixUtils.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/PluginProcessor.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/ShelfFilter.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/SineWave.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/SmoothDelay.h"
  .         .         .         "${CMAKE_CURRENT_LIST_DIR}/LevelDetector.h"
  x         .         .         "${CMAKE_CURRENT_LIST_DIR}/FDN.cpp"
  x         .         .         "${CMAKE_CURRENT_LIST_DIR}/ShelfFilter.cpp"
  x         .         .         "${CMAKE_CURRENT_LIST_DIR}/SmoothDelay.cpp"
  x         .         .         "${CMAKE_CURRENT_LIST_DIR}/LevelDetector.cpp"
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

add_subdirectory(r8lib)

jucer_export_target(
    "Xcode (MacOSX)"
    EXTERNAL_LIBRARIES_TO_LINK r8lib
)

jucer_export_target_configuration(
    "Xcode (MacOSX)"
    NAME "Debug"
    DEBUG_MODE ON
    BINARY_NAME "FDNShared"
)

jucer_export_target_configuration(
    "Xcode (MacOSX)"
    NAME "Release"
    DEBUG_MODE OFF
    BINARY_NAME "FDNShared"
)

jucer_export_target(
    "Visual Studio 2017"
    EXTRA_PREPROCESSOR_DEFINITIONS
        _CRT_SECURE_NO_WARNINGS
    EXTERNAL_LIBRARIES_TO_LINK r8lib
)

jucer_export_target_configuration(
    "Visual Studio 2017"
    NAME "Debug"
    DEBUG_MODE ON
    BINARY_NAME "FDNShared"
)

jucer_export_target_configuration(
    "Visual Studio 2017"
    NAME "Release"
    DEBUG_MODE OFF
    BINARY_NAME "FDNShared"
)

jucer_export_target(
    "Linux Makefile"
    EXTERNAL_LIBRARIES_TO_LINK r8lib
)

jucer_export_target_configuration(
    "Linux Makefile"
    NAME "Debug"
    DEBUG_MODE ON
    BINARY_NAME "FDNShared"
)

jucer_export_target_configuration(
    "Linux Makefile"
    NAME "Release"
    DEBUG_MODE OFF
    BINARY_NAME "FDNShared"
)

jucer_project_end()

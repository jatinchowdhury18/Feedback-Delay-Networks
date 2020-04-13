#!/bin/bash

# exit if any command fails
set -e

if [[ $1 = "--help" || $1 = "" ]]; then
    echo "To create a new plugin project, run ./create.sh <Name of project>"
    echo "To run the project after creation, run ./create.sh <Name of project> --run"
    echo "The project name must not contain any spaces"
    exit
fi

echo "Creating plugin project for $1..."

if [ -d $1 ]; then
    echo "Folder of this name already exists... aborting!"
    exit
fi

# check for dependencies
if [ ! -d modules/JUCE/modules ]; then
    echo "JUCE modules folder not located, make sure you are running from the root of the repo,and have run the setup script."
    exit
fi

if [ ! -d modules/FRUT/cmake ]; then
    echo "FRUT cmake folder not located, make sure you are running from the root of the repo,and have run the setup script."
    exit
fi

# create project folder
mkdir $1 && cd $1

# write cmake list
echo "Creating CMake list..."

# cmake header
echo "cmake_minimum_required(VERSION 3.4)" > CMakeLists.txt
echo "" >> CMakeLists.txt
echo "project(\"$1\")" >> CMakeLists.txt
echo "" >> CMakeLists.txt
echo "# FRUT stuff" >> CMakeLists.txt
echo "list(APPEND CMAKE_MODULE_PATH \"\${CMAKE_CURRENT_LIST_DIR}/../modules/FRUT/prefix/FRUT/cmake\")" >> CMakeLists.txt
echo "include(Reprojucer)" >> CMakeLists.txt
echo "" >> CMakeLists.txt
echo "# Comment this line to build without the reverb tester" >> CMakeLists.txt
echo "add_definitions(-DBUILD_WITH_REVERB_TESTER)" >> CMakeLists.txt
echo "" >> CMakeLists.txt

# cmake jucer project
proj_id=$( cat /dev/urandom | env LC_CTYPE=C tr -cd 'A-Za-z0-9' | head -c 5)
plug_id=$( cat /dev/urandom | env LC_CTYPE=C tr -cd 'A-Za-z0-9' | head -c 4)
comp="Kim and Jatin"
bund_id="kimandjatin"

echo "jucer_project_begin(" >> CMakeLists.txt
echo "    JUCER_VERSION \"5.4.7\"" >> CMakeLists.txt
echo "    PROJECT_FILE \"\${$1_jucer_FILE}\"" >> CMakeLists.txt
echo "    PROJECT_ID \"${proj_id}\"" >> CMakeLists.txt
echo ")" >> CMakeLists.txt
echo "" >> CMakeLists.txt

echo "jucer_project_settings(" >> CMakeLists.txt
echo "    PROJECT_NAME \"$1\"" >> CMakeLists.txt
echo "    PROJECT_VERSION \"1.0.0\"" >> CMakeLists.txt
echo "    COMPANY_NAME \"${comp}\"" >> CMakeLists.txt
echo "    COMPANY_COPYRIGHT \"${comp}\"" >> CMakeLists.txt
echo "    REPORT_JUCE_APP_USAGE OFF" >> CMakeLists.txt
echo "    DISPLAY_THE_JUCE_SPLASH_SCREEN OFF" >> CMakeLists.txt
echo "    PROJECT_TYPE \"Audio Plug-in\"" >> CMakeLists.txt
echo "    BUNDLE_IDENTIFIER \"com.${bund_id}.$1\"" >> CMakeLists.txt
echo "    CXX_LANGUAGE_STANDARD \"C++17\"" >> CMakeLists.txt
echo "    HEADER_SEARCH_PATHS" >> CMakeLists.txt
echo "        \${CMAKE_CURRENT_LIST_DIR}/../shared/" >> CMakeLists.txt
echo ")" >> CMakeLists.txt
echo "" >> CMakeLists.txt

echo "jucer_audio_plugin_settings(" >> CMakeLists.txt
echo "    PLUGIN_FORMATS" >> CMakeLists.txt
echo "        VST3" >> CMakeLists.txt
echo "        AU" >> CMakeLists.txt
echo "        Standalone" >> CMakeLists.txt
echo "    PLUGIN_NAME \"$1\"" >> CMakeLists.txt
echo "    PLUGIN_DESCRIPTION \"$1\"" >> CMakeLists.txt
echo "    PLUGIN_MANUFACTURER \"${comp}\"" >> CMakeLists.txt
echo "    PLUGIN_MANUFACTURER_CODE \"KAJ\"" >> CMakeLists.txt
echo "    PLUGIN_CODE \"${plug_id}\"" >> CMakeLists.txt
echo "    PLUGIN_AU_EXPORT_PREFIX \"$1AU\"" >> CMakeLists.txt
echo ")" >> CMakeLists.txt
echo "" >> CMakeLists.txt

echo "Adding source files..."
echo "# Source files" >> CMakeLists.txt
# shared files
echo "jucer_project_files(\"$1/shared\"" >> CMakeLists.txt
echo "# Compile   Xcode     Binary    File" >> CMakeLists.txt
echo "#           Resource  Resource" >> CMakeLists.txt
for file in ../shared/*.h; do
    echo "  .         .         .         \"\${CMAKE_CURRENT_LIST_DIR}/${file}\"" >> CMakeLists.txt
done
for file in ../shared/*.cpp; do
    echo "  x         .         .         \"\${CMAKE_CURRENT_LIST_DIR}/${file}\"" >> CMakeLists.txt
done
echo ")" >> CMakeLists.txt
echo "" >> CMakeLists.txt

# ReverbTester files
echo "get_directory_property(MYDEFS COMPILE_DEFINITIONS)" >> CMakeLists.txt
echo "if(MYDEFS MATCHES \"^BUILD_WITH_REVERB_TESTER\")" >> CMakeLists.txt
echo "    MESSAGE(STATUS \"Building with ReverbTester\")" >> CMakeLists.txt
echo "    jucer_project_files(\"$1/ReverbTester\"" >> CMakeLists.txt
echo "    # Compile   Xcode     Binary    File" >> CMakeLists.txt
echo "    #           Resource  Resource" >> CMakeLists.txt
for file in ../ReverbTester/Source/*.h; do
    echo "      .         .         .         \"\${CMAKE_CURRENT_LIST_DIR}/${file}\"" >> CMakeLists.txt
done
for file in ../ReverbTester/Source/*.cpp; do
    echo "      x         .         .         \"\${CMAKE_CURRENT_LIST_DIR}/${file}\"" >> CMakeLists.txt
done
echo ")" >> CMakeLists.txt
echo "else()" >> CMakeLists.txt
echo "    MESSAGE(STATUS \"Building without ReverbTester\")" >> CMakeLists.txt
echo "endif()" >> CMakeLists.txt
echo "" >> CMakeLists.txt

# source files
echo "jucer_project_files(\"$1/Source\"" >> CMakeLists.txt
echo "# Compile   Xcode     Binary    File" >> CMakeLists.txt
echo "#           Resource  Resource" >> CMakeLists.txt
echo "  .         .         .         \"\${CMAKE_CURRENT_LIST_DIR}/Source/$1Processor.h\"" >> CMakeLists.txt
echo "  x         .         .         \"\${CMAKE_CURRENT_LIST_DIR}/Source/$1Processor.cpp\"" >> CMakeLists.txt
echo ")" >> CMakeLists.txt
echo "" >> CMakeLists.txt

# JUCE modules
echo "Adding JUCE modules..."
echo "# JUCE modules" >> CMakeLists.txt
modules="juce_audio_basics juce_audio_devices juce_audio_plugin_client juce_audio_formats juce_audio_processors juce_audio_utils juce_core juce_data_structures juce_dsp juce_events juce_graphics juce_gui_basics juce_gui_extra"
for mod in $modules; do
    echo "jucer_project_module(" >> CMakeLists.txt
    echo "    $mod" >> CMakeLists.txt
    echo "    PATH \"\${CMAKE_CURRENT_LIST_DIR}/../modules/JUCE/modules\"" >> CMakeLists.txt
    if [ $mod = "juce_audio_plugin_client" ]; then
        echo "JUCE_VST3_CAN_REPLACE_VST2 OFF" >> CMakeLists.txt
    fi
    echo ")" >> CMakeLists.txt
    echo "" >> CMakeLists.txt
done

# JUCE exporters
echo "Configuring exporters..."
declare -a exports=("Xcode (MacOSX)" "Visual Studio 2017" "Linux Makefile")
for e in "${exports[@]}"; do
    echo "jucer_export_target(" >> CMakeLists.txt
    echo "    \"$e\"" >> CMakeLists.txt
    if [ "$e" == "Visual Studio 2017" ]; then
        echo "    EXTRA_PREPROCESSOR_DEFINITIONS" >> CMakeLists.txt
        echo "        _CRT_SECURE_NO_WARNINGS" >> CMakeLists.txt
    fi
    echo ")" >> CMakeLists.txt
    echo "" >> CMakeLists.txt
    echo "jucer_export_target_configuration(" >> CMakeLists.txt
    echo "    \"$e\"" >> CMakeLists.txt
    echo "    NAME \"Debug\"" >> CMakeLists.txt
    echo "    DEBUG_MODE ON" >> CMakeLists.txt
    echo "    BINARY_NAME \"$1\"" >> CMakeLists.txt
    echo ")" >> CMakeLists.txt
    echo "" >> CMakeLists.txt
    echo "jucer_export_target_configuration(" >> CMakeLists.txt
    echo "    \"$e\"" >> CMakeLists.txt
    echo "    NAME \"Release\"" >> CMakeLists.txt
    echo "    DEBUG_MODE OFF" >> CMakeLists.txt
    echo "    BINARY_NAME \"$1\"" >> CMakeLists.txt
    echo ")" >> CMakeLists.txt
    echo "" >> CMakeLists.txt
done

echo "jucer_project_end()" >> CMakeLists.txt
echo "CMake list complete!"

# Create template files
echo "Creating template files..."
mkdir Source/ && cd Source/
class=$1Processor
proc_h=$class.h
proc_cpp=$class.cpp

# Write Processor.h
echo "#pragma once" >> $proc_h
echo "" >> $proc_h
echo "#include \"PluginProcessor.h\"" >> $proc_h
echo "" >> $proc_h
echo "class $class : public PluginProcessor<${class}>" >> $proc_h
echo "{" >> $proc_h
echo "public:" >> $proc_h
echo "    $class();" >> $proc_h
echo "    ~$class();" >> $proc_h
echo "    " >> $proc_h
echo "    static void addParameters (Parameters& params);" >> $proc_h
echo "    void prepareToPlay (double sampleRate, int samplesPerBlock) override;" >> $proc_h
echo "    void releaseResources() override;" >> $proc_h
echo "    void processBlock (AudioBuffer<float>& buffer) override;" >> $proc_h
echo "    " >> $proc_h
echo "private:" >> $proc_h
echo "    " >> $proc_h
echo "    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR ($class)" >> $proc_h
echo "};" >> $proc_h

# Write Processor.cpp
echo "#include \"${proc_h}\"" >> $proc_cpp
echo "" >> $proc_cpp
echo "${class}::${class}()" >> $proc_cpp
echo "{" >> $proc_cpp
echo "}" >> $proc_cpp
echo "" >> $proc_cpp
echo "${class}::~${class}()" >> $proc_cpp
echo "{" >> $proc_cpp
echo "}" >> $proc_cpp
echo "" >> $proc_cpp
echo "void ${class}::addParameters (Parameters& params)" >> $proc_cpp
echo "{" >> $proc_cpp
echo "}" >> $proc_cpp
echo "" >> $proc_cpp
echo "void ${class}::prepareToPlay (double sampleRate, int samplesPerBlock)" >> $proc_cpp
echo "{" >> $proc_cpp
echo "    AudioProcessor::setRateAndBufferSizeDetails (sampleRate, samplesPerBlock);" >> $proc_cpp
echo "}" >> $proc_cpp
echo "" >> $proc_cpp
echo "void ${class}::releaseResources()" >> $proc_cpp
echo "{" >> $proc_cpp
echo "}" >> $proc_cpp
echo "" >> $proc_cpp
echo "void ${class}::processBlock (AudioBuffer<float>& buffer)" >> $proc_cpp
echo "{" >> $proc_cpp
echo "}" >> $proc_cpp
echo "" >> $proc_cpp
echo "// This creates new instances of the plugin.." >> $proc_cpp
echo "#ifdef BUILD_WITH_REVERB_TESTER // Set this flag to run with ReverbTester" >> $proc_cpp
echo "#include \"../../ReverbTester/Source/ReverbTesterProcessor.h\"" >> $proc_cpp
echo "AudioProcessor* JUCE_CALLTYPE createPluginFilter()" >> $proc_cpp
echo "{" >> $proc_cpp
echo "    return new ReverbTesterProcessor (new ${class}());" >> $proc_cpp
echo "}" >> $proc_cpp
echo "#else" >> $proc_cpp
echo "AudioProcessor* JUCE_CALLTYPE createPluginFilter()" >> $proc_cpp
echo "{" >> $proc_cpp
echo "    return new ${class}();" >> $proc_cpp
echo "}" >> $proc_cpp
echo "#endif" >> $proc_cpp

# test build
echo "Running build..."
cd ../
mkdir build/ && cd build/
if [[ $OSTYPE == "msys" || $OSTYPE == "win32" ]]; then
    cmake .. -G "Visual Studio 15 2017 Win64"
else
    cmake ..
fi
cmake --build .
if [[ $2 = "--run" ]]; then
    if [[ $OSTYPE == "msys" || $OSTYPE == "win32" ]]; then
        exe="Debug/Standalone Plugin/$1.exe"
    fi
    if [[ "$OSTYPE" == "darwin"* ]]; then
        exe="$1.app/Contents/MacOS/$1"
    fi
    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        exe="./$1"
    fi
    "${exe}" &
fi

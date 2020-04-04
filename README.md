# Feedback Delay Networks

[![Build Status](https://travis-ci.com/jatinchowdhury18/Feedback-Delay-Networks.svg?token=Ub9niJrqG1Br1qaaxp7E&branch=master)](https://travis-ci.com/jatinchowdhury18/Feedback-Delay-Networks)

Research project looking at feedback delay network reverbs
effects with nonlinear and/or time-varying elements. This
repo contains realtime plugins implementing these reverbs,
as well as a test bench for developing new effects.

## Instructions

### Dependencies
This project depends on
[JUCE](https://github.com/WeAreROLI/JUCE),
[FRUT](https://github.com/McMartin/FRUT),
and [CMake](https://cmake.org). FRUT and JUCE
are included in this repository as submodules.
However, CMake must be installed by the user.

### Installation
To install the build chain for these FDN reverbs,
simply clone the repo and run the setup script.
```bash
$ git clone https://github.com/jatinchowdhury18/Feedback-Delay-Networks.git
$ cd Feedback-Delay-Networks
$ ./setup.sh
```

### Building a plugin
As an example of how to build a plugin from this
repo, we'll walk through the example process of
building the BaseFDN plugin.

```bash
# Starting from the repo root
$ cd BaseFDN
$ mkdir build && cd build/
$ cmake ..
$ cmake --build .
```
The plugin will be built as a Standalone
Application, a VST3 plugin, and (on Mac only)
an AU plugin.

By default, the plugins will be built with the
ReverbTester test bench. To compile without the
test bench, change the compiler flag at the
bottom of the `BaseFDNProcessor.cpp` file.

```cpp
// This creates new instances of the plugin..
#if 1 // Set this flag to run with ReverbTester
#include "../../ReverbTester/Source/ReverbTesterProcessor.h"
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbTesterProcessor (new BaseFDNProcessor());
}
#else
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BaseFDNProcessor();
}
#endif
```

### Creating a new plugin
To create a new plugin, use the `create.sh` script in
the root of the repository. For more information run
`./create.sh --help`.

## Papers

JOS (from [Physical Audio Signal Processing](https://ccrma.stanford.edu/~jos/pasp/Feedback_Delay_Networks_FDN.html))

Sebastien Schlecht, Emanuel Habets, "Practical Considerations
of Time-Varying Feedback Delay Networks"
([pdf](http://www.aes.org/e-lib/browse.cfm?elib=17679))

Davide Rocchesso, Julius Smith, "Circulant and Elliptic Feedback
Delay Networks for Artificial Reverberation"
([link](https://ccrma.stanford.edu/~jos/cfdn/))

Karolina Prawda, Vesa Valimaki, Sebastien Schlecht,
"Improved Reverberation Time Control for Feedback Delay
Networks" ([pdf](http://dafx2019.bcu.ac.uk/papers/DAFx2019_paper_46.pdf))

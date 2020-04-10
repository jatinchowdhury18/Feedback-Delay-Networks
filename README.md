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
test bench, comment out the definition at the
top of the `CMakeLists.txt` file.

```cmake
# Comment this line to build without the reverb tester
add_definitions(-DBUILD_WITH_REVERB_TESTER)
```

### Creating a new plugin
To create a new plugin, use the `create.sh` script in
the root of the repository. For more information run
`./create.sh --help`.

### Suggested Workflow

If you would like to contribute to the repository, 
please complete your work on a separate branch.
When your work is ready to be merged, you may create
a pull request. Once all automated checks have passed 
and/or your code has been reviewed, your pull request 
will be merged.

```bash
# Create new branch and push to origin
$ git checkout -b mybranch
$ git push -u origin mybranch

# Do some work...

# Review your changes
$ git status

# Add your changes
$ git add MyNewFolder/
$ git add MyNewFile.cpp

# Commit your changes
$ git commit -m "Did some work"

# Push your changes
$ git push
```

In order to avoid merge conflicts, please rebase
before creating a new branch, as well as before
pushing changes from your branch.

```bash
$ git fetch origin
$ git rebase origin/master
```

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

Jean-Marc Jot, Antoine Chaigne, "Digital Delay Networks
For Designing Artifical Reverberators" ([link](http://www.aes.org/e-lib/browse.cfm?elib=5663))

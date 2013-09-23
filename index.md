---
layout: default
title: A VR library
---

###Introduction

MinVR is an open-source cross-platform virtual reality library initially developed at the University of Minnesota [Interactive Visualization Lab](http://ivlab.cs.umn.edu). It supports interaction with multiple VR input devices and display configurations (e.g. fish-tank VR displays, powerwalls, and CAVE systems), while allowing for integration with a variety of graphics toolkits (e.g. raw OpenGL, G3D, Glut, OpenSceneGraph, etc.).

###MinVR Features and Design 

Core Library Features:
- window configuration
- stereo output for various display devices
- input device configuration
- input events for VR, multi-touch, and other non-keyboard, mouse, joystick events based on VRPN
- setting up head tracking and appropriate projection matrices   
- multi-threaded rendering with support for multiple graphics windows (i.e., multiple graphics contexts), one per thread

###Documentation

[View the API documentation for the latest release](docs/latest/index.html)

Tutorials and guides:
- [Compiling MinVR and its dependencies](docs/latest/compiling.html)
- [Using MinVR in your project](docs/latest/using.html)
- [Specifying a VR configuration](docs/latest/vrsetup.html)
- [Handling MinVR events](docs/latest/events.html)
- [Creating a new AppKit](docs/latest/creatingappkits.html)
- [Contributing to MinVR](docs/latest/contributing.html)

###Questions and Issues

If you have a question about how to use MinVR, a bug report to submit, or a feature request, please submit an issue or pull request on the [MinVR Repository issue tracker](https://github.com/MinVR/MinVR/issues).

To suggest changes to this website, create a pull request for the `gh-pages` branch of the repository. For directions on how to contribute code and issue a pull request, see the tutorial on [Contributing to MinVR](docs/latest/contributing.html).

###License and History

MinVR is derived from the [VRG3D project](https://sourceforge.net/projects/vrg3d) which provides a VR wrapper around the [G3D graphics library](http://g3d.sourceforge.net). Derived classes are copyright to the Regents of the University of Minnesota and Brown University. MinVR uses the same license as VRG3D and G3D.

MinVR is licensed under the BSD-3 Clause License. This allows you to do almost anything you want with the MinVR code as long as you provide attribution, don’t hold us liable, and do not use our names for marketing purposes. A copy of the license can be found in the `LICENSE.txt` file and in every header file in the source code.

###Authors and Contributors

- Bret Jackson [@bretjackson](http://github.com/bretjackson)
- Daniel F. Keefe

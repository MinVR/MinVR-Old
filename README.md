#MinVR

**This repository has been deprecated. It is replaced by a new version called MinVR2: https://github.com/ivlab/minvr2**

##Introduction

MinVR is an open-source cross-platform virtual reality library initially developed at the University of Minnesota [Interactive Visualization Lab](http://ivlab.cs.umn.edu). It supports interaction with multiple VR input devices and display configurations (e.g. fish-tank VR displays, powerwalls, and CAVE systems), while allowing for integration with a variety of graphics toolkits (e.g. raw OpenGL, G3D, Glut, OpenSceneGraph, etc.).

The main MinVR website is found at [http://minvr.github.io/MinVR/](http://minvr.github.io/MinVR/).

##MinVR Features and Design 

Core Library Features:
- window configuration
- stereo output for various display devices
- input device configuration
- input events for VR, multi-touch, and other non-keyboard, mouse, joystick events based on VRPN
- setting up head tracking and appropriate projection matrices   
- multi-threaded rendering with support for multiple graphics windows (i.e., multiple graphics contexts), one per thread

###Dependencies and Design Philosophy

MinVR's primary design philosophy is to minimize potential dependency conflicts with other libraries you may be using. The core library depends on [Boost](http://boost.org), the header-only OpenGL Math Library (http://glm.g-truc.net), and [VRPN](http://www.cs.unc.edu/Research/vrpn/) and [TUIO](http://http://www.tuio.org/) for connecting with input devices.

In addition to the core library, individual App Kits are included for interfacing with a variety of common graphics libraries. Examples for each AppKit are included in their respective directories.

##Documentation

For API documentation and tutorials, see the [MinVR Documentation](http://minvr.github.io/MinVR/docs/latest/index.html).



# Introduction				{#mainpage}
The Windows Template Framework (WTF) is a lightning fast, light-weight, header-only GUI library for Windows written in C++11. It's designed to be easy to use and quick to setup for a quick-and-dirty Windows GUI application. The architecture is highly modular with maximum code reuse by leveraging modern template meta-programming techniques.

WTF is largely an architectural and feasibility experiment. Maximum reuse is one of the motivating factors behind this library. There are many GUI toolkits around and they all seem to share the trait of code bloat and complex configuration. GUI toolkits present an interesting challenge because the various components and widgets have a mix-and-match composition of behaviors.  For example, a button contains a label that needs to produce click events while a drop-down box has several labels, produces click events and needs to editable, while a text box is editable but produces no click events. The various GUI components share a hodge-podge of behaviors and a variety of programming techniques have been employed in toolkits to minimize the maintenance effort but most fall short IMO.

WTL, for example, should be more properly named Windows Macro Library than a template library but does a fairly good job at reducing duplication.  wxWidgets has lots of duplicate code but their goal is a cross platform toolkit.  It's clumsy and difficult to learn.  Qt has a nice programmer's interface but dont peek behind the sheets if you want to keep your lunch down. WinForms is easy to code but requires a terrabyte of framework libraries, CLI interop is required to do anything native and its slow as hell. GTK is for linux. The proper response to the current state of native GUI toolkits on Windows is WTF!


# Getting Started
Add the wtf folder to the project's include path and include `wtf.hpp` in a compilation unit. That's it! There is nothing to compile (except for your application.) and there are no additional binaries to link or distribute.

To generate the documentation you will need:
 - [Cmake](http://www.cmake.org) to generate example programs and configure the docs.
 - [Doxygen](http://www.doxygen.org) to generate source documentation.
 - [Graphviz](http://www.graphviz.org) (optional) to generate charts and graphs in the documentation.

The first place to visit for learning WTF is the documentation. [Using WTF](using_wtf.md) gives an overview of how to build a user interface with WTF.

For a deep dive into the structure and architecture take a look at [The Hierarchy Generator Pattern](hierarchy_gen.md) which is the backbone of the framework. WTF uses a specialized hierarchy generator pattern for [Message Handling](message_handling.md).


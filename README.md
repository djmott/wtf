### Introduction
The Windows Template Framework (WTF) is a header-only GUI library for Windows written in C++11. It's designed to be easy to use and quick to setup a GUI application. The architecture is highly modular with maximum code reuse by leveraging modern template meta-programming techniques.

I hate duplicating code as every software engineer should. Maximum reuse is one of the motivating factors behind this library. There are many GUI toolkits around and they all seem to share the same trait of having far more code than is necessary. GUI toolkits present an interesting challenge because the various components and widgets have a mix-and-match composition of behaviors.  For example, a button contains a label that needs to produce click events while a drop-down box has several labels, produces click events and needs to editable, while a text box is editable but produces no click events. The various GUI components share a hodge-podge of behaviors and a variety of programming techniques have been employed in the various toolkits to minimize the maintenance effort but most fall short IMO.

WTL for example should be more properly named Windows Macro Library than a template library but does a fairly good job at reducing duplication.  wxWidgets has lots of duplicate code but their goal is a cross platform toolkit.  It's clumsy and difficult to learn.  Qt has a nice programmer's interface but dont peek behind the sheets if you want to keep your lunch down. WinForms is easy to code but requires a terrabyte of framework libraries, CLI interop to do anything native and its slow as hell. GTK is for linux. The proper response to the current state of GUI toolkits is WTF!


### Getting Started
Add the wtf folder to the project's include path and include wtf.hpp in a compilation unit. Here's a minimal example:


~~~cpp
#include "wtf.hpp"

int main(){
  wtf::default_theme::form oForm;
  oForm.show();
  wtf::message oMsg;
  return oMsg.pump();
}
~~~

This creates a generic form and shows it then begins the main message pump.  To make things more exciting the forms and controls can be sub-classed and methods overridden to accomplish various tasks.  Here's a slightly more involved example:

~~~cpp
#include "wtf.hpp"

namespace theme = wtf::default_theme;

struct MyForm : theme::form{
  
  MyForm() : oButton(*this){
    oButton.move(10, 10, 150, 25);
    oButton.text(L"Hello World");
    oButton.OnClickEvent = [](){ std::cout << "Button clicked"; };
  }

  theme::button oButton;
};

int main(){
  MyForm oForm;
  oForm.show();
  wtf::message oMsg;
  return oMsg.pump();
}
~~~

Here a form is sub-classed and it contains a button. One important thing to notice is the constructor of the button accepts the form as the parent e.g. `oButton(*this)` There are a number of ways to enhance the behavior and get event notifications depending on what you're trying to do. The producible events currently use `std::function` for callbacks to _user side_ events. Most of the behaviors from an internal perspective can be modified by providing overridden functions. More advanced widgets use the template framework to generate compositional patterns of various behavioral policies to create new, feature rich GUI components.

Currently the controls and themes are limited but I'll be adding them as time permits.

### Architecture

#### The Problem
So you have some code to draw a border around a control and you want to use it in a button, label and text box. You also have some code to type text into a control and you want to include it in a text box and combo box. You also want to collect click events from a button and combo box but you don't want to duplicate any of the code. The trouble comes when you try to compose these behaviors into concrete widgets using multiple inheritance and the behavior policies need to communicate with each other.  For example, the combo box might need to respond to a click event to show the list contents which makes the click event and the list display tightly coupled. The coupling might be reversed in another widget so you've got some trouble on your hands.

#### The Solution
WTF composes discrete behavioral policies in self-contained classes that can be combine to create rich components. The code which draws a border is shared with all the widgets that draw borders.  The code that produces click events is shared by all the widgets that produce click events and so on. The behaviors are chained together in a __linear class hierarchy__ at compile time to avoid multiple inheritance.  The behavior policies use a parameterized super class template idiom which is successively composed in a linear hierarchy at compile time. Here a few simple behaviors found in `behaviors.hpp`:

~~~cpp
  template <typename _SuperT> struct has_close : _SuperT{
    void close() { CloseWindow(*this); }
  };

  template <typename _SuperT> struct has_show : _SuperT{
    void show(){ ::ShowWindow(*this, SW_SHOW); }
    void hide(){ ::ShowWindow(*this, SW_HIDE); }
  };
  
  template <typename _SuperT> struct has_move : _SuperT{
    void move(int x, int y, int width, int height, bool repaint = true){
      wtf::exception::throw_lasterr_if(::MoveWindow(*this, x, y, width, height, repaint ? TRUE : FALSE), [](BOOL b){return !b; });
    }
  };
~~~

The parameterized super class idiom is a bit ugly but it certainly beats the alternative. Widgets inherit these behaviors by listing them during declaration:

~~~cpp
struct NewWidget : window<NewWidget, has_close, has_show, has_move>...
~~~

The window class template composes these behaviors in __linerarly__ to form something akin to:

~~~cpp
struct NewWidget : window<NewWidget, has_close<window<has_show<window<has_move<window<>>>>>>>...
~~~

As the behavior count increases it can be to follow the construction when created manually so the `window` template takes care of constructing the hierarchy while keeping the declaration succinct and clear.

The `window` template collects all the policies into a linear class hierarchy with the top-most super class being a `window<>` specialization that holds the HWND.


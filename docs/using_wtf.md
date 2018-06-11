# Using WTF

## A simple example

Here's a minimal example:

~~~cpp
#include "wtf/wtf.hpp"
using namespace wtf;
int main(){
  return form().run();
}
~~~



This creates a generic form and shows it then begins the main message pump. To make things more exciting, the forms and controls can be sub-classed and methods overridden to accomplish various tasks.  Here's a slightly more involved example:

~~~cpp
#include "wtf/wtf.hpp"
using namespace wtf;
struct MyForm : form{

  MyForm() : oPanel(this)
  {
    OnCreate.connect([this](){ 
      oPanel.border_style(border_styles::double_raised);
      oPanel.move(10, 10, 150, 25);
    });
  }

  panel oPanel;
};

int main(){
  return MyForm().run();
}

~~~

Here a form is sub-classed and it contains a panel. An important thing to note is the constructor of the panel which accepts a pointer to the parent form as it's argument e.g. `oPanel(this)`. Every child widget must be constructed in this manner to work properly. The _user side_ is notified of events using a callback mechanism. In this example a lambda is invoked when the form's window is created to change the panel's border and position it.  The callback mechanism is very flexible permitting a variety of ways to execute actions when various UI events occur.

Callback objects exist for all the typically expected events. Zero or more _targets_ can be invoked by the callback objects when the event occurs. Targets are connected to callback objects with the `connect` method and can include lambdas, static methods and class members:

~~~cpp
void StaticMethod(){ std::cout << "StaticMethod target invoked" << std::endl; }

struct MyForm : form{
  MyForm(){
    OnCreate.connect(&StaticMethod);
    OnCreate.connect<MyForm, &MyForm::ClassMember>(this);
    OnCreate += [](){ std::cout << "Lambda target invoked" << std::endl; };
  }

  void ClassMember() { std::cout << "ClassMember target invoked" << std::endl; }
};

int main(){ 
  return MyForm().run(); 
};
~~~

Here the various callback targets are demonstrated. The last target is a lambda which is connected with the `+=` operator short hand which is equivelent to calling the `connect` method. All the targets are invoked when the form's window is created and produces the following output:
<pre>
StaticMethod target invoked
ClassMember target invoked
Lambda target invoked
</pre>
*NOTE: there is no guarantee on the order in which targets are invoked when multiple targets are connected to the same callback.*

# Architecture
The WTF library is partitioned into two primary layers.
1. __policy layer__ - descrete and self contained behavioral policies 
2. __user interface layer__ - GUI widgets composition of elements from the policy layer for library consumers

## The Policy Layer
Each item in the policy layer is a descrete decoration or behavioral unit. The composition of these elements produce the feature rich UI widgets with minimal code duplication.

#### The Problem
So you have some code to draw a border around a control and you want to use it in a button, label and text box. You also have some code to type text into a control and you want to include it in a text box and combo box. You also want to collect click events from a button and combo box but you don't want to duplicate any of the code. The trouble comes when you try to compose these behaviors into concrete widgets using multiple inheritance and the behavior policies need to communicate with each other.  For example, the combo box might need to respond to a click event to show the list contents which makes the click event and the list display tightly coupled. The coupling might be reversed in another widget so you've got some trouble on your hands.

#### The Solution
WTF composes discrete policies in self-contained classes that can be combine to create rich components. The code which draws a border is shared with all the widgets that draw borders.  The code that produces click events is shared by all the widgets that produce click events and so on. The behaviors are chained together in a __linear class hierarchy__ at compile time to avoid multiple inheritance.  The policies use a parameterized super class template idiom which is successively composed in a linear hierarchy at compile time. Here a few simple behaviors:

~~~cpp
  template <typename _SuperT> struct has_close : _SuperT{
    void close() { ::CloseWindow(*this); }
  };

  template <typename _SuperT> struct has_show : _SuperT{
    void show(){ ::ShowWindow(*this, SW_SHOW); }
    void hide(){ ::ShowWindow(*this, SW_HIDE); }
  };
  
  template <typename _SuperT> struct has_move : _SuperT{
    void move(int x, int y, int width, int height, bool repaint = true){
      ::MoveWindow(*this, x, y, width, height, repaint ? TRUE : FALSE);
    }
  };
~~~

The parameterized super class idiom is a bit ugly but it certainly beats the alternative. Widgets inherit these behaviors by listing them during declaration:

~~~cpp
struct NewWidget : window_impl<NewWidget, has_close, has_show, has_move>...
~~~
The NewWidget inherits from all the policies which results in a hierarchy like:

![class hierarchy](docs/images/example_hierarchy_1.png)

The `window_impl` class template collects all the policies into a linear class hierarchy with the top-most super class being a `window<void>` specialization that holds the HWND. There is a little bit more going on with the WTF implementation but for this illustration it's sufficient and useful. 

There are a number of places where ease of code and architecture were chosen over performance but they're not of much concern.  I normally frown on virtual method calls particularly in process intensive paths but this is a GUI toolkit afterall and it's not intended render 120fps. Despite the inefficiencies it will probably skill smoke a number of other similar toolkits in the performance arena. I'm certain it will make a mockery of Qt in the performance and footprint department.  WinForms will be several light-years behind.



### Painting
WTF widgets are owner drawn. To improve performance a single device context is created when window receives a WM_PAINT or WM_ERASEBKGND message is received and it's reused by the policy templates. This is the traditional approach but the modular architecture of WTF present a few challenges here and there. In keeping with a uniform design goal the WM_PAINT and WM_ERASEBKGND message parameters are slightly modified:

|  message  |  parameter  |  description  |
|-----------|-------------|---------------|
| WM_PAINT | wparam | `device_context*`|
| WM_PAINT | lparam | `paint_struct*` |
| WM_ERASEBKGND | wparam |`device_context*`|

Casting to and from raw pointers makes my skin crawl but it's legacy C anyway so there's little chance of avoiding it entirely. The native WM_PAINT message doesn't use either wparam or lparam so the liberty was taken to repurpose them. The native WM_ERASEBKGND passes the native device context in the wparam so it's been replaced with the convenience wrapper.

### Thread Safety
There is none. Native Windows GDI APIs aren't supposed to be run from multiple threads for the most part. In most cases, the thread that creates a GUI window must be the thread to control it and close it. Running background threads is fine but the main thread that creates the window should control the window for it's lifetime.  Bring your own synchronization.

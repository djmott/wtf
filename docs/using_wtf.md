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


### Thread Safety
There is none. Native Windows GDI APIs aren't supposed to be run from multiple threads for the most part. In most cases, the thread that creates a GUI window must be the thread to control it and close it. Running background threads is fine but the main thread that creates the window should control the window for it's lifetime.  Bring your own synchronization.

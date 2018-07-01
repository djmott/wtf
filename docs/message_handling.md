# WTF Message Handling

## Introduction

Windows applications process notification messages sent to the application from the system. Notification messages include such things as click events, resize event, mouse move events, etc. All messages for a window class are posted to the same message handler for processing. 

### The window_message class

The window_message class contains the information necessary to process Windows messages. It contains the HWND, message identifier, WPARAM, LPARAM and LRESULT members familiar to API programmers. These members are used in the same way that the Windows SDK suggests. In addition, the **window_message** class contains a *bhandled* member that identifies whether or not to forward the message to the default window handler. This member will be explained later.

### Generic Handler

The **window** interface is the base interface of all WTF forms and widgets and has the following member:

~~~cpp
virtual void handle_msg(wtf::window_message& msg) = 0;
~~~
Each policy that is interested in processing a message provides an override of the **handle_msg** method. The policies are careful to only process the specific message of interest. For example, the **wtf::messages::wm_timer** policy has the following override:

~~~cpp
void handle_msg(wtf::window_message& msg) override {
    if (WM_TIMER == msg.umsg) on_wm_timer(static_cast<UINT_PTR>(msg.wparam));
}
~~~

The **wtf::messages::wm_timer** policy calls the **on_wm_timer** member function only when a WM_TIMER message is recieved.

### Message Propogation

The **handle_msg** method is called on each policy in the generated class hierarchy starting with the concrete specialization, up the inheritance chain, and ending with the window interface. If no policy handles the message it is sent to the Windows default message handler. A policy can set the *bhandled* member of the **window_message** class to prevent further processing by other policies and prevent forwarding to the default handler.
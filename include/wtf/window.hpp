/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace _ {
    template <class, class> struct _;
  }
  
  /** @class window base class of all windows
  */
  struct window{
    /// an implementation may use different window styles 
    static const DWORD ExStyle = WS_EX_NOPARENTNOTIFY;
    static const DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP;

    window(const window&) = delete;
    window& operator=(const window&) = delete;

    virtual ~window(){ if (_handle) ::DestroyWindow(_handle); }

    window() : _parent(nullptr), _handle(nullptr){}

    window& operator=(window&& src){
      std::swap(_parent, src._parent);
      std::swap(_handle, src._handle);
      std::swap(_children, src._children);
      return *this;
    }

    window(window&& src){ *this = std::move(src); }

    explicit window(window * Parent) : _parent(Parent), _handle(nullptr){
      if (Parent) Parent->_children.push_back(this);
    }

    const window * const parent() const{ return _parent; }

    const std::vector<window*>& children() const{ return _children; }

    virtual const std::type_info& type() const = 0;

    HWND operator*() const{ return _handle; }
    operator HWND() const{ return _handle; }

    void add(window*pChild){
      assert(pChild && !pChild->_handle);
      _children.push_back(pChild);
      if (_handle){
        if (pChild->_handle){
          ::SetParent(*pChild, *this);
        } else{
          pChild->run();
        }
      }
    }

    virtual void handle_msg(wtf::window_message& msg) {
      if (msg.bhandled) return;
      if (msg.umsg == WM_CLOSE) {
        DestroyWindow(msg.hwnd);
        _handle = nullptr;
      }
      msg.lresult = DefWindowProc(msg.hwnd, msg.umsg, msg.wparam, msg.lparam);
    }

    callback<void(window *)> OnCreated;

  protected:

    template <typename, template <typename> typename...> friend class window_impl;
    template <class, class> friend struct _::_;

    window * _parent;
    HWND _handle;
    std::vector<window*> _children;

    virtual int run() { return  0; }

    //this is different than WM_CREATE, its not part of windows and called by exec after CreateWindow returns
    virtual void on_wm_created(){ OnCreated(this); }


  };
  


}

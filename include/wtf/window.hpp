/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  
  /** @class window base class of all windows
  */
  struct window{
    using vector = std::vector<window*>;
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
      return *this;
    }

    window(window&& src){ *this = std::move(src); }

    explicit window(window *) : _handle(nullptr){
    }

    const window * const parent() const{ return _parent; }

    vector children() const{ 
      vector oRet;
      exception::throw_lasterr_if(EnumChildWindows(_handle, enum_children, reinterpret_cast<LPARAM>(&oRet)), [](BOOL b) {return FALSE == b; });
      return oRet;
    }

    virtual const std::type_info& type() const = 0;

    HWND operator*() const{ return _handle; }
    operator HWND() const{ return _handle; }

    void add(window*pChild){
      assert(pChild && !pChild->_handle);
      if (pChild->_handle){
        ::SetParent(*pChild, *this);
      } else{
        pChild->run();
      }
    }

    callback<void(window *)> OnCreated;

  protected:

    template <typename, template <typename> typename...> friend class window_impl;

    window * _parent;
    HWND _handle;


    static BOOL CALLBACK enum_children(HWND hwnd, LPARAM lParam) {
      auto pRet = reinterpret_cast<vector*>(lParam);
      auto pChild = exception::throw_lasterr_if(reinterpret_cast<window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA)), [](window*p) {return nullptr == p; });
      pRet->push_back(pChild);
      return TRUE;
    }

    virtual int run() { return  0; }

    //this is different than WM_CREATE, its not part of windows and called by exec after CreateWindow returns
    virtual void on_wm_created(){ OnCreated(this); }


    virtual void handle_msg(_::window_message& msg){
      if (msg.bhandled) return;
      if (msg.umsg == WM_CLOSE){
        DestroyWindow(msg.hwnd);
        _handle = nullptr;
      }
      msg.lresult = ::DefWindowProc(msg.hwnd, msg.umsg, msg.wparam, msg.lparam);
    }
  };
  


}

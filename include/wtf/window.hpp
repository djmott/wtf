/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#if !defined(WTF_DEBUG_MESSAGES)
  #define WTF_DEBUG_MESSAGES 0
#endif

namespace wtf{

 
  /** 
  @class window 
  @brief Super class of all widgets and forms
  */ 
  struct window{
    template <typename, template <typename> typename...> friend struct window_impl;

    /// an implementation may use different window styles 
    static constexpr DWORD ExStyle = WS_EX_NOPARENTNOTIFY;
    static constexpr DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP;

    virtual ~window() { if (_handle) ::DestroyWindow(_handle); _handle = nullptr; }

    window(const window&) = delete;

    window& operator=(const window&) = delete;

    window() {
#if WTF_USE_COMMON_CONTROLS
      wtf::_::init_common_controls::get();
#endif
    }

    window& operator=(window&& src) noexcept {
      std::swap(_parent, src._parent);
      std::swap(_handle, src._handle);
      std::swap(_children, src._children);
      std::swap(OnCreated, src.OnCreated);
      return *this;
    }

    window(window&& src) noexcept { *this = std::move(src); }

    const window * const parent() const noexcept { return _parent; }

    const std::vector<window*>& children() const noexcept { return _children; }

    virtual const std::type_info& type() const noexcept = 0;

    virtual HWND handle() const { return _handle; }

    HWND operator*() const noexcept { return _handle; }

    operator HWND() const noexcept { return _handle; }

    template <typename _child_t>
    void add(_child_t& oChild){
      _children.push_back(static_cast<window*>(&oChild));
      oChild._parent = this;
      if (_handle){
        _children.push_back(static_cast<window*>(&oChild));
        if (oChild._handle) {
          ::SetParent(*oChild, *this);
        } else{
          oChild.run();
        }
      }
    }

    callback<void(window *)> OnCreated;

    virtual int run() = 0;

  protected:

    window * _parent = nullptr;
    HWND _handle = nullptr;
    std::vector<window*> _children;

    virtual void on_created() { OnCreated(this); }
      
    virtual void handle_msg(wtf::window_message& msg) = 0;

    virtual void fwd_msg(wtf::window_message&, const std::type_info&) = 0;
  };
  



  template <typename _impl_t, template <typename> typename _head_t, template <typename> typename..._tail_t>
  struct window_impl<_impl_t, _head_t, _tail_t...> :  _head_t<window_impl<_impl_t, _tail_t...>> {
    
  protected:
    template <typename, template <typename> typename...> friend struct window_impl;

    void fwd_msg(wtf::window_message& msg, const std::type_info& last_handler) override {
      using super = _head_t<window_impl<_impl_t, _tail_t...>>;
      if (msg.bhandled) return;
      if (last_handler == typeid(&super::handle_msg)){
        super::fwd_msg(msg, last_handler);
      }else{
#if WTF_DEBUG_MESSAGES
        if (WM_CREATE == msg.umsg) {
          tstring sMsg = _T("Forwarding message to ") + to_tstring(typeid(super).name()) + _T("\n");
          OutputDebugString(sMsg.c_str());
        }
#endif
        super::handle_msg(msg);
        if (msg.bhandled) return;
        super::fwd_msg(msg, typeid(&super::handle_msg));
      }
    }
  };



  template <typename _impl_t> struct window_impl<_impl_t> : window {

    const std::type_info& type() const noexcept final { return typeid(_impl_t); }

    int run() override {
      window::_handle = wtf::exception::throw_lasterr_if(
        ::CreateWindowEx(_impl_t::ExStyle, window_class_type::get().name(), nullptr, _impl_t::Style,
          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (window::_parent ? window::_parent->_handle : nullptr),
          nullptr, instance_handle(), this), [](HWND h) noexcept { return nullptr == h; });
      this->on_created();
      return 0;
    }

  protected:
    template <typename, template <typename> typename...> friend struct window_impl;

    void handle_msg(wtf::window_message& msg) override {}

    void fwd_msg(wtf::window_message& msg, const std::type_info&) override {
      if (msg.bhandled) return;
      if (msg.hwnd != _handle) return;
      msg.lresult = CallWindowProc(window_class_type::get().default_window_proc(), msg.hwnd, msg.umsg, msg.wparam, msg.lparam);
      msg.bhandled = true;
      if (WM_DESTROY == msg.umsg) {
        SetWindowLongPtr(msg.hwnd, window_class_type::get().window_extra_offset(), 0);
        _handle = nullptr;
        _parent = nullptr;
      }
    }

    /* messages arrive here from windows then are propagated from the implementation, through the
    * inheritance chain and back through all the handle_message overrides in order from the
    * bottom most inherited (_ImplT::handle_message) to top most parent (this class::handle_message)
    */
    static LRESULT CALLBACK window_proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {

#if WTF_DEBUG_MESSAGES
      auto sMsg = to_tstring(reinterpret_cast<size_t>(hwnd)) + _T(" ");
      sMsg += to_tstring(typeid(_impl_t).name());
      sMsg += _T(" ") + wtf::_::msg_name(umsg) + _T("\n");
      OutputDebugString(sMsg.c_str());
#endif

      try {
        _impl_t * pThis = nullptr;

        if (WM_NCCREATE == umsg || WM_CREATE == umsg) {
          auto pCreate = reinterpret_cast<CREATESTRUCT*>(lparam);
          assert(pCreate);
          pThis = reinterpret_cast<_impl_t*>(pCreate->lpCreateParams);
          assert(pThis);
          pThis->_handle = hwnd;
          SetWindowLongPtr(hwnd, window_class_type::get().window_extra_offset(), reinterpret_cast<LONG_PTR>(pThis));
          if (WM_CREATE == umsg) {
            for (auto pChild : pThis->children()) {
              pChild->run();
            }
          }
        } else {
          pThis = reinterpret_cast<_impl_t*>(GetWindowLongPtr(hwnd, window_class_type::get().window_extra_offset()));
        }

        if (!pThis) return CallWindowProc(window_class_type::get().default_window_proc(), hwnd, umsg, wparam, lparam);

        wtf::window_message msg{ hwnd, umsg, wparam, lparam, false, 0 };

        if (WM_COMMAND == umsg || 
          WM_NOTIFY == umsg || 
          WM_MENUSELECT == umsg ||
          WM_INITMENU == umsg ||
          WM_EXITMENULOOP == umsg || 
          WM_ENTERMENULOOP == umsg ||
          WM_MENUCOMMAND == umsg) {
          //legacy control notification messages sent to parent window. Forward them back to the originating control
          const HWND hTarget = (WM_NOTIFY == umsg ? reinterpret_cast<const NMHDR*>(lparam)->hwndFrom : reinterpret_cast<HWND>(lparam));
          for (const auto & pChild : pThis->children()) {
            if ((WM_COMMAND == umsg || WM_NOTIFY == umsg) && hTarget != pChild->handle()) continue;
            pChild->handle_msg(msg);
            pChild->fwd_msg(msg, typeid(bool));
            break;
          }
          if (WM_NOTIFY == umsg) {
            msg.bhandled = false;
          }
        };
        if (!msg.bhandled) pThis->fwd_msg(msg, typeid(bool));
        if (!msg.bhandled) return CallWindowProc(window_class_type::get().default_window_proc(), hwnd, umsg, wparam, lparam);
        return msg.lresult;
      }
      catch (const wtf::exception& ex) {
        tstring sMsg = to_tstring(ex.what());
        sMsg += _T("\n");
        sMsg += to_tstring(ex.code());
        auto iRet = message_box::exec(hwnd, sMsg.c_str(), _T("An exception occurred."), message_box::buttons::cancel_retry_continue, message_box::icons::stop);
        if (message_box::response::cancel == iRet) abort();
        if (message_box::response::retry == iRet) return -1;
        throw;
      }
    }

    using window_class_type = window_class<_impl_t, &window_impl<_impl_t>::window_proc>;

  };


}

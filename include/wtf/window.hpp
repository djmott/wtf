﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#if !defined(WTF_DEBUG_MESSAGES)
  #define WTF_DEBUG_MESSAGES 0
#endif

#define DOXY_INHERIT_WINDOW

namespace wtf{
 
  /** 
  @class window 
  @brief Super class of all controls and forms
  @details This class is inherited as the final super-class during hierarchy generation
  */ 
  struct window{
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

    //! @brief Invoked after the window and all child windows have been created
    callback<void(window *)> OnCreated;
    //! @brief Creates the window and child windows
    virtual int run() = 0 { return 0; };
    //! @brief Returns the parent window or nullptr for top-level forms
    virtual const window * const parent() const noexcept { return _parent; }
    //! @brief collection of child windows
    virtual const std::vector<window*>& children() const noexcept { return _children; }
    //! @brief returns the type info of the concrete implementation
    virtual const std::type_info& type() const noexcept = 0;

    //! @brief provides access to the underlying window handle
    //!@{
    virtual HWND handle() const { return _handle; }

    virtual HWND operator*() const noexcept { return _handle; }

    virtual operator HWND() const noexcept { return _handle; }
    //!@}
    //!@brief Adds a child window to the children collection
    //!@details Child windows are created when run() is invoked on the parent.
    //!This results in recursive parent/child window creation, typically as a result of run() invoked on a form.
    virtual void add(window& oChild) {
      _children.push_back(static_cast<window*>(&oChild));
      oChild._parent = this;
      if (_handle) {
        _children.push_back(static_cast<window*>(&oChild));
        if (oChild._handle) {
          ::SetParent(*oChild, *this);
        }
        else {
          oChild.run();
        }
      }
    }

  protected:
    window(HWND hwnd) : _handle(hwnd){}
    template <typename, template <typename> typename...> friend struct window_impl;

    window * _parent = nullptr;
    HWND _handle = nullptr;
    std::vector<window*> _children;

    //! @brief The windows extended style
    DWORD _exstyle = WS_EX_NOPARENTNOTIFY;
    //! @brief The windows style
    DWORD _style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;

    //! @brief Indicates the window and all child windows have been created.
    virtual void on_created() { OnCreated(this); }
    //! @brief Handle various window messages
    //! @param[in,out] msg Details of the message
    virtual void handle_msg(wtf::window_message& msg) = 0;
#if !DOXY_INVOKED
    //! @internal
    virtual void fwd_msg(wtf::window_message&, const std::type_info&) = 0;
    //! @endinternal
#endif
  };
  

  /** @class window_impl
  @brief Specialization of window_impl that participates in the hierarchy generation pattern.
  @details Policies are recursively inherited by inheriting this class and passing one or more policies as template parameters
  @tparam _impl_t The concrete implementation
  @tparam _head_t,_tail_t List of policies to inherit
  */
  template <typename _impl_t, template <typename> typename _head_t, template <typename> typename..._tail_t>
  struct window_impl<_impl_t, _head_t, _tail_t...> : DOXY_INHERIT_WINDOW _head_t<window_impl<_impl_t, _tail_t...>> {
    
  protected:
    template <typename, template <typename> typename...> friend struct window_impl;

    /** @brief forwards messages to inherited policies
    @details Messages are forwarded to each policy in the generated hierarchy. Policies may or may not override handle_msg.
    @param[in,out] msg The window message
    @param[in] last_handler is used to determine whether or not handle_msg has been invoked on a policy for a given message.
    It prevents handle_msg from being invoked twice on a super-class when _head_t does not override handle_msg.
    */
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

  /** @class window_impl
  @brief Specialization of window_impl that participates in the hierarchy generation pattern.
  @details This is the final generator that is selected when no policies are specified. It causes the concrete impelemntation to inherit from window.
  @tparam _impl_t The concrete implementation
  */
  template <typename _impl_t> struct window_impl<_impl_t> : window {

    //! @brief returns the type info of the concrete implementation
    const std::type_info& type() const noexcept final { return typeid(_impl_t); }

    //! @brief Creates the window and child windows
    int run() override {
      const auto & oWC = _impl_t::window_class_type<window_proc>::get();
      window::_handle = wtf::exception::throw_lasterr_if(
        ::CreateWindowEx(window::_exstyle, oWC.lpszClassName, nullptr, window::_style,
          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (window::_parent ? window::_parent->_handle : nullptr),
          nullptr, instance_handle(), this), [](HWND h) noexcept { return nullptr == h; });
      this->on_created();
      return 0;
    }

  protected:
    template <typename, template <typename> typename...> friend struct window_impl;

    /** @brief The main message handler of WTF controls and forms.
    @details messages arrive here from windows then are propagated from the implementation, through the
    inheritance hierarchy and back through all the handle_message overrides in order from the
    bottom most inherited (_ImplT::handle_message) to top most parent (this class::handle_message)
    */
    static LRESULT CALLBACK window_proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {

#if WTF_DEBUG_MESSAGES
      auto sMsg = to_tstring(reinterpret_cast<size_t>(hwnd)) + _T(" ");
      sMsg += to_tstring(typeid(_impl_t).name());
      sMsg += _T(" ") + wtf::_::msg_name(umsg) + _T("\n");
      OutputDebugString(sMsg.c_str());
#endif
      using wc = typename _impl_t::window_class_type<window_proc>;

      try {
        _impl_t * pThis = nullptr;

        if (WM_NCCREATE == umsg || WM_CREATE == umsg) {
          auto pCreate = reinterpret_cast<CREATESTRUCT*>(lparam);
          assert(pCreate);
          pThis = reinterpret_cast<_impl_t*>(pCreate->lpCreateParams);
          assert(pThis);
          pThis->_handle = hwnd;
          SetWindowLongPtr(hwnd, wc::get().window_extra_offset(), reinterpret_cast<LONG_PTR>(pThis));
          if (WM_CREATE == umsg) {
            for (auto pChild : pThis->children()) {
              pChild->run();
            }
          }
        }
        else {
          pThis = reinterpret_cast<_impl_t*>(GetWindowLongPtr(hwnd, wc::get().window_extra_offset()));
        }

        if (!pThis) return CallWindowProc(wc::get().default_window_proc(), hwnd, umsg, wparam, lparam);

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
        if (!msg.bhandled) return CallWindowProc(wc::get().default_window_proc(), hwnd, umsg, wparam, lparam);
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

    void handle_msg(wtf::window_message& msg) override {}

    void fwd_msg(wtf::window_message& msg, const std::type_info&) override {
      if (msg.bhandled) return;
      if (msg.hwnd != _handle) return;
      auto & oWC = typename _impl_t::window_class_type<window_proc>::get();
      msg.lresult = CallWindowProc(oWC.default_window_proc(), msg.hwnd, msg.umsg, msg.wparam, msg.lparam);
      msg.bhandled = true;
      if (WM_DESTROY == msg.umsg) {
        SetWindowLongPtr(msg.hwnd, typename _impl_t::window_class_type<window_proc>::get().window_extra_offset(), 0);
        _handle = nullptr;
        _parent = nullptr;
      }
    }



  };
}

/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

 
  /** @class window super class of all windows
  */ 
  struct window{
    /// an implementation may use different window styles 
    static constexpr DWORD ExStyle = WS_EX_NOPARENTNOTIFY;
    static constexpr DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP;

    window(const window&) = delete;
    window& operator=(const window&) = delete;

    virtual ~window(){ if (_handle) ::DestroyWindow(_handle); }

    window() noexcept : _parent(nullptr), _handle(nullptr){}

    window& operator=(window&& src) noexcept {
      std::swap(_parent, src._parent);
      std::swap(_handle, src._handle);
      std::swap(_children, src._children);
      return *this;
    }

    window(window&& src) noexcept { *this = std::move(src); }

    explicit window(window * Parent)  : _parent(Parent), _handle(nullptr){
      if (Parent) Parent->_children.push_back(this);
    }

    const window * const parent() const noexcept { return _parent; }

    const std::vector<window*>& children() const noexcept { return _children; }

    virtual const std::type_info& type() const noexcept = 0;

    HWND operator*() const noexcept { return _handle; }
    operator HWND() const noexcept { return _handle; }

    virtual void add(window*pChild){
      _children.push_back(pChild);
      if (_handle){
        if (pChild->_handle){
          ::SetParent(*pChild, *this);
        } else{
          pChild->run();
        }
      }
    }

    //this is different than WM_CREATE, its not part of windows and called by run after CreateWindow returns
    //virtual void on_wm_created(){ OnCreated(this); }
    callback<void(window *)> OnCreated;

    virtual int run() = 0;

    template <typename, template <typename> typename...> friend struct window_impl;

  protected:

    window * _parent = nullptr;
    HWND _handle = nullptr;
    std::vector<window*> _children;

    virtual void on_created() { OnCreated(this); }
    
  private:

    virtual void handle_msg(wtf::window_message& ) = 0;
    virtual void fwd_msg(wtf::window_message&, const std::type_info&) = 0;
  };
  



  template <typename _impl_t, template <typename> typename _head_t, template <typename> typename..._tail_t>
  struct window_impl<_impl_t, _head_t, _tail_t...> :  _head_t<window_impl<_impl_t, _tail_t...>> {
    template <typename, template <typename> typename...> friend struct window_impl;

    template <typename ... _arg_ts> window_impl(_arg_ts&&...args) noexcept : _head_t<window_impl<_impl_t, _tail_t...>>(std::forward<_arg_ts>(args)...) {}

  private:

    void fwd_msg(wtf::window_message& msg, const std::type_info& last_handler) override {
      using super = _head_t<window_impl<_impl_t, _tail_t...>>;
      if (msg.bhandled) return;
      if (last_handler == typeid(&super::handle_msg)){
        super::fwd_msg(msg, last_handler);
      }else{
#if _DEBUG
        if (WM_CREATE == msg.umsg) {
          tstring sMsg = _T("Forwarding message to ") + to_tstring(typeid(super).name()) + _T("\n");
          OutputDebugString(sMsg.c_str());
        }
#endif
        super::handle_msg(msg);
        super::fwd_msg(msg, typeid(&super::handle_msg));
      }
    }
  };

  template <typename _impl_t> struct window_impl<_impl_t> : window {

    const std::type_info& type() const noexcept final { return typeid(_impl_t); }

    template <typename ... _arg_ts> window_impl(_arg_ts&&...args) : window(std::forward<_arg_ts>(args)...) {}

    int run() override {
      window::_handle = wtf::exception::throw_lasterr_if(
        ::CreateWindowEx(_impl_t::ExStyle, window_class_type::get().name(), nullptr, _impl_t::Style,
          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (window::_parent ? window::_parent->_handle : nullptr),
          nullptr, instance_handle(), this), [](HWND h) noexcept { return nullptr == h; });
      this->on_created();
      return 0;
    }

  private:
    template <typename, template <typename> typename...> friend struct window_impl;

    void handle_msg(wtf::window_message& msg) override {}

    void fwd_msg(wtf::window_message& msg, const std::type_info&) override {
      msg.lresult = CallWindowProc(window_class_type::get().default_window_proc(), msg.hwnd, msg.umsg, msg.wparam, msg.lparam);
    }

    /* messages arrive here from windows then are propagated from the implementation, through the
    * inheritance chain and back through all the handle_message overrides in order from the
    * bottom most inherited (_ImplT::handle_message) to top most parent (this class::handle_message)
    */
    static LRESULT CALLBACK window_proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {

#if defined(_DEBUG)
      auto sMsg = to_tstring(GetTickCount()) + _T(" ");
      sMsg += to_tstring(reinterpret_cast<size_t>(hwnd)) + _T(" ");
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
          SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
          if (WM_CREATE == umsg) {
            for (auto pChild : pThis->children()) {
              pChild->run();
            }
          }
        } else {
          pThis = reinterpret_cast<_impl_t*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (!pThis) return DefWindowProc(hwnd, umsg, wparam, lparam);

        wtf::window_message msg{ hwnd, umsg, wparam, lparam, false, 0 };

        if ((WM_COMMAND == umsg && lparam) || (WM_NOTIFY == umsg)) {
          //legacy control notification messages sent to parent window. Forward them back to the originating control
          const HWND hTarget = (WM_NOTIFY == umsg ? reinterpret_cast<const NMHDR*>(lparam)->hwndFrom : reinterpret_cast<HWND>(lparam));
          for (const auto & pChild : pThis->children()) {
            if (hTarget != pChild->_handle) continue;
            pChild->fwd_msg(msg, typeid(bool));
            break;
          }
        };
        if (!msg.bhandled) pThis->fwd_msg(msg, typeid(bool));
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

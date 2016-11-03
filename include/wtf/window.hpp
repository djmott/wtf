/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  
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
          pChild->exec();
        }
      }
    }

    callback<void()> OnCreated;

  protected:

    template <typename, template <typename> typename...> friend class window_impl;


    window * _parent;
    HWND _handle;
    std::vector<window*> _children;

    virtual int exec(){ return 0; }

    //this is different than WM_CREATE, its not part of windows and called by exec after CreateWindow returns
    virtual void on_wm_created(){ OnCreated(); }


    virtual void handle_msg(_::window_message& msg){
      if (msg.bhandled) return;
      if (msg.umsg == WM_CLOSE){
        DestroyWindow(msg.hwnd);
        _handle = nullptr;
      }
      msg.lresult = DefWindowProc(msg.hwnd, msg.umsg, msg.wparam, msg.lparam);
    }
  };
  


  namespace policy{
    template <typename> struct isa_tab_container;
  }

  template <typename _ImplT, template <typename> class..._Policies>
  class window_impl : public wtf::_::normalized_policies<_ImplT, _Policies...>::type{
    using __super_t = typename wtf::_::normalized_policies<_ImplT, _Policies...>::type;
  
  public:
  
    template <typename ... _ParamTs> window_impl(_ParamTs&&...oParam) : __super_t(std::forward<_ParamTs>(oParam)...){}
  
  protected:

    friend struct window;
    template <typename> friend struct policy::isa_tab_container;

    void handle_msg(_::window_message& msg) override{
      __super_t::handle_msg(msg);
    }

    const std::type_info& type() const override{ return typeid(_ImplT); }


    int exec() override{
      _handle = wtf::exception::throw_lasterr_if(
        ::CreateWindowEx(_ImplT::ExStyle, window_class_type::get().name(), nullptr, _ImplT::Style,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (_parent ? _parent->_handle : nullptr),
        nullptr, _::instance_handle(), this), [](HWND h){ return nullptr == h; });
      window::on_wm_created();
      return 0;
    }


    /* messages arrive here from windows then are propagated from the implementation, through the
    * inheritance chain and back through all the handle_message overrides in order from the
    * bottom most inherited (_ImplT::handle_message) to top most parent (this class::handle_message)
    */
    static LRESULT CALLBACK window_proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){

    #if __WTF_DEBUG_MESSAGES__
      std::cout << GetTickCount() << " " << typeid(_ImplT).name() << " " << _::msg_name(umsg) << '\n';
    #endif

      try{
        _ImplT * pThis = nullptr;

        if (WM_NCCREATE == umsg){
          auto pCreate = reinterpret_cast<CREATESTRUCT*>(lparam);
          assert(pCreate);
          pThis = reinterpret_cast<_ImplT*>(pCreate->lpCreateParams);
          assert(pThis);
          pThis->_handle = hwnd;
          SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
          for (auto pChild : pThis->children()){
            pChild->exec();
          }
        } else{
          pThis = reinterpret_cast<_ImplT*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (!pThis) return DefWindowProc(hwnd, umsg, wparam, lparam);

        _::window_message msg{ hwnd, umsg, wparam, lparam, false, 0 };

        if (WM_ERASEBKGND == umsg){
          auto oDC = wtf::_::device_context::get_client(hwnd);
          msg.lparam = reinterpret_cast<LPARAM>(&oDC);
          pThis->handle_msg(msg);
          return msg.lresult;
        } else if (WM_PAINT == umsg){
          RECT r;
          if (0 == GetUpdateRect(hwnd, &r, FALSE)){
            return DefWindowProc(hwnd, umsg, wparam, lparam);
          }
          _::paint_struct oPaint(*pThis);
          auto oDC = wtf::_::device_context::get_client(hwnd);
          msg.wparam = reinterpret_cast<WPARAM>(&oDC);
          msg.lparam = reinterpret_cast<LPARAM>(&oPaint);
          pThis->handle_msg(msg);
          return msg.lresult;
        } else{
          pThis->handle_msg(msg);
          return msg.lresult;
        }
      }
      catch (const wtf::exception& ex){
        tstring sMsg = _T("");
        std::string swhat(ex.what());
        std::string scode(ex.code());
        //poor mans utf8 conversion until mingw supports codecvt
        std::copy(swhat.begin(), swhat.end(), std::back_inserter(sMsg));
        sMsg += _T("\n");
        std::copy(scode.begin(), scode.end(), std::back_inserter(sMsg));
        auto iRet = message_box::exec(hwnd, sMsg.c_str(), _T("An exception occurred."), message_box::buttons::cancel_retry_continue, message_box::icons::stop);
        if (message_box::response::cancel == iRet) abort();
        if (message_box::response::retry == iRet) return -1;
        throw;
      }
    }

    using window_class_type = _::window_class_ex<_ImplT, &window_impl<_ImplT, _Policies...>::window_proc>;
  };

}

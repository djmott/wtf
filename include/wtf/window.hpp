#pragma once

namespace wtf{



  /** Specialization that terminates the inheritance hierarchy
  * super most base class of all windows
  */
  template <> struct window<void>{

    window(const window&) = delete;
    window& operator=(const window&) = delete;

    virtual ~window(){}

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

    virtual const type_info& type() const = 0;

    HWND operator*() const{ return _handle; }
    operator HWND() const{ return _handle; }

    void add(window*pChild){
      assert(pChild && !pChild->_handle);
      _children.push_back(pChild);
      if (_handle) pChild->exec();
    }

  protected:
    template <typename, template <typename, typename> class ... > friend struct window;

    virtual int exec() = 0;

    window * _parent;
    HWND _handle;
    std::vector<window*> _children;
  };



  template <typename _ImplT, template <typename, typename> class _HeadT, template <typename, typename> class ... _TailT>
  struct window<_ImplT, _HeadT, _TailT...> : _HeadT<window<_ImplT, _TailT...>, _ImplT>{
    using _super_t = _HeadT<window<_ImplT, _TailT...>, _ImplT>;
    using window_type = window<_ImplT, _HeadT, _TailT...>;

    window(const window&) = delete;
    window& operator=(const window&) = delete;

    virtual ~window() = default;

    window(){}
    window(window&& src) : _super_t(std::move(src)){}
    explicit window(window<void> * pParent) : _super_t(pParent){}
    window& operator=(window&& src){ return _super_t::operator=(std::move(src)); }

  private:
    template <typename, template <typename, typename> class ... > friend struct window;

    template <template <typename, typename> class ... _NewPolicyTs> using add_policy = window<_ImplT, _HeadT, _TailT..., _NewPolicyTs...>;

    LRESULT handle_message(HWND, UINT, WPARAM, LPARAM, bool&){ return 0; }

    LRESULT propagate_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& handled){
      auto handler_ret = _super_t::handle_message(hwnd, umsg, wparam, lparam, handled);
      if (handled) return handler_ret;
      return _super_t::propagate_message(hwnd, umsg, wparam, lparam, handled);
    }
  };




  template <typename _ImplT> struct window<_ImplT> : window<void>{

    using _super_t = window<void>;
    using window_type = window<_ImplT>;


    /// an implementation may want to use different window styles so add their definitions as class wide static constants
    static const DWORD ExStyle = WS_EX_NOPARENTNOTIFY;
    static const DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP;

    virtual ~window(){ if (_handle) ::DestroyWindow(_handle); }

    window() = default;
    window(const window&) = delete;
    window(window&& src) : _super_t(std::move(src)){}
    explicit window(window<void> * parent) : _super_t(parent){}

    window& operator=(const window&) = delete;
    window& operator=(window&& src){ return _super_t::operator=(std::move(src)); }



    virtual const type_info& type() const override{ return typeid(_ImplT); }

  protected:

    template <template <typename, typename> class ... _NewPolicyTs> using add_policy = window<_ImplT, _NewPolicyTs...>;

    template <typename, template <typename, typename> class ... > friend struct window;
    template <typename, DWORD, DWORD> friend struct form_base;

    virtual int exec() override{
      _handle = wtf::exception::throw_lasterr_if(::CreateWindowEx(
        _ImplT::ExStyle,
        window_class_type::get().name(),
        nullptr,
        _ImplT::Style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (_parent ? _parent->_handle : nullptr),
        nullptr,
        _::instance_handle(),
        this
      ), [](HWND h){ return nullptr == h; });
      return 0;
    }

    LRESULT handle_message(HWND, UINT, WPARAM, LPARAM, bool&){ return 0; }

    LRESULT propagate_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& handled){

    #if __WTF_DEBUG_MESSAGES__
      std::string sTemp = typeid(_ImplT).name();
      sTemp += " ";
      sTemp += _::msg_name(umsg);
      sTemp += " default handler";
      std::cout << sTemp << std::endl;
    #endif


      switch (umsg){
        case WM_CLOSE:
          DestroyWindow(hwnd); _handle = nullptr; break;
        default:
          return DefWindowProc(hwnd, umsg, wparam, lparam);
      }
      return 0;
    }

    /* messages arrive here from windows then are propagated from the implementation, through the
    * inheritance chain and back through all the handle_message overrides in order from the
    * bottom most inherited (_ImplT::handle_message) to top most parent (this class::handle_message)
    */
    static LRESULT CALLBACK window_proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){

    #if __WTF_DEBUG_MESSAGES__
      std::string sTemp = typeid(_ImplT).name();
      sTemp += " ";
      sTemp += _::msg_name(umsg);
      std::cout << sTemp << std::endl;
    #endif

      try{
        _ImplT * pThis = nullptr;
        bool handled = false;

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

        if (WM_ERASEBKGND == umsg){
          auto oDC = wtf::device_context::get_client(hwnd);

          return pThis->propagate_message(hwnd, umsg, wparam, reinterpret_cast<LPARAM>(&oDC), handled);

        } else if (WM_PAINT == umsg){
          RECT r;
          if (0 == GetUpdateRect(hwnd, &r, FALSE)){
            return DefWindowProc(hwnd, umsg, wparam, lparam);
          }
          paint_struct oPaint(*pThis);
          auto oDC = wtf::device_context::get_client(hwnd);

          return pThis->propagate_message(hwnd, umsg, reinterpret_cast<WPARAM>(&oDC), reinterpret_cast<LPARAM>(&oPaint), handled);

        } else{
          return pThis->propagate_message(hwnd, umsg, wparam, lparam, handled);
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

    using window_class_type = _::window_class_ex<_ImplT, &window<_ImplT>::window_proc>;

  };
}

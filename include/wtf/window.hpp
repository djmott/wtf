#pragma once

namespace wtf{

  template <typename _ImplT> class window;
  
  /** Specialization that terminates the inheritance hierarchy
  * super-most base class of all windows
  */
  template <> class window<void>{    
  public:
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
      if (_handle) pChild->exec();
    }

  protected:

	  template <typename _ImplT> friend class window;

    /// an implementation may use different window styles 
    static const DWORD ExStyle = WS_EX_NOPARENTNOTIFY;
    static const DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP;


    window * _parent;
    HWND _handle;
    std::vector<window*> _children;

    virtual int exec() = 0;

  };

  template <typename _ImplT> class window : public iwindow{
  public:
    
	  explicit window(iwindow * Parent) : iwindow(Parent) {}
  protected:
    virtual void handle_msg(window_message& msg) {
      if (msg.bhandled) return;
#if __WTF_DEBUG_MESSAGES__
      std::cout << _::msg_name(msg.umsg) << " default handler" << std::endl;
#endif
      if (msg.umsg == WM_CLOSE) {
        DestroyWindow(msg.hwnd);
        _handle = nullptr;
      }
      msg.lresult = DefWindowProc(msg.hwnd, msg.umsg, msg.wparam, msg.lparam);
    }
       
    const std::type_info& type() const override{ return typeid(_ImplT); }

    int exec() override {
      _handle = wtf::exception::throw_lasterr_if(
            ::CreateWindowEx(_ImplT::ExStyle, window_class_type::get().name(), nullptr, _ImplT::Style,
            CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,(_parent ? _parent->_handle : nullptr),
            nullptr,_::instance_handle(),this), [](HWND h){ return nullptr == h; });
      return 0;
    }


    /* messages arrive here from windows then are propagated from the implementation, through the
    * inheritance chain and back through all the handle_message overrides in order from the
    * bottom most inherited (_ImplT::handle_message) to top most parent (this class::handle_message)
    */
    static LRESULT CALLBACK window_proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {

#if __WTF_DEBUG_MESSAGES__
      std::string sTemp = typeid(_ImplT).name();
      sTemp += " ";
      sTemp += _::msg_name(umsg);
      std::cout << sTemp << std::endl;
#endif

      try {
        _ImplT * pThis = nullptr;

        if (WM_NCCREATE == umsg) {
          auto pCreate = reinterpret_cast<CREATESTRUCT*>(lparam);
          assert(pCreate);
          pThis = reinterpret_cast<_ImplT*>(pCreate->lpCreateParams);
          assert(pThis);
          pThis->_handle = hwnd;
          SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
          for (auto pChild : pThis->children()) {
            pChild->exec();
          }
        } else {
          pThis = reinterpret_cast<_ImplT*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (!pThis) return DefWindowProc(hwnd, umsg, wparam, lparam);

        window_message msg{ hwnd, umsg, wparam, lparam, false, 0 };

        if (WM_ERASEBKGND == umsg) {
          auto oDC = wtf::device_context::get_client(hwnd);
          msg.lparam = reinterpret_cast<LPARAM>(&oDC);
          pThis->handle_msg(msg);
          return msg.lresult;
        } else if (WM_PAINT == umsg) {
          RECT r;
          if (0 == GetUpdateRect(hwnd, &r, FALSE)) {
            return DefWindowProc(hwnd, umsg, wparam, lparam);
          }
          paint_struct oPaint(*pThis);
          auto oDC = wtf::device_context::get_client(hwnd);
          msg.wparam = reinterpret_cast<WPARAM>(&oDC);
          msg.lparam = reinterpret_cast<LPARAM>(&oPaint);
          pThis->handle_msg(msg);
          return msg.lresult;
        } else {
          pThis->handle_msg(msg);
          return msg.lresult;
        }
      }
      catch (const wtf::exception& ex) {
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

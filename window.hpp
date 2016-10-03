#pragma once

namespace wtf{
  namespace _{
    template <template <typename> typename _DeclT, template <typename> typename ... _TailT> struct has_policy;

    template <template <typename> typename _DeclT, template <typename> typename ... _TailT>
    struct has_policy<_DeclT, _DeclT, _TailT...>{ static const bool value = true; };

    template <template <typename> typename _DeclT>
    struct has_policy<_DeclT>{ static const bool value = false; };

    template <template <typename> typename _DeclT, template <typename> typename _HeadT, template <typename> typename ... _TailT>
    struct has_policy<_DeclT, _HeadT, _TailT...>{ static const bool value = has_policy<_DeclT, _TailT...>::value; };
  }


  template <typename _ImplT, template <typename> typename ... _PolicyListT>
  struct window : base_window<_ImplT, _PolicyListT...>{
    using _base_window_t = base_window<_ImplT, _PolicyListT...>;
    using window_type = window<_ImplT, _PolicyListT...>;

    window(HWND hParent, bool bCreate = true) : _base_window_t(){
      //its neccessary for all the constructors in the inheritance chain to initialize before creating the window
      //otherwise the vtbl wont point to intended overloads
      if (bCreate) window::create(hParent);
    }

    virtual ~window() = default;

    template <template <typename> typename _PolicyT> bool has_policy() const{ return _::has_policy<_PolicyT, _PolicyListT...>::value; }
  };

  template <typename _ImplT, template <typename> typename _HeadT, template <typename> typename ... _TailT>
  struct base_window<_ImplT, _HeadT, _TailT...> : _HeadT<base_window<_ImplT, _TailT...>>{
    using _super_t = _HeadT<base_window<_ImplT, _TailT...>>;
    using window_type = base_window<_ImplT, _HeadT, _TailT...>;

    base_window() = default;
    virtual ~base_window() = default;

  private:
    template <typename, template <typename> typename ... > friend struct base_window;

    virtual LRESULT propogate_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& handled) override{
      auto handler_ret = _super_t::handle_message(hwnd, umsg, wparam, lparam, handled);
      if (handled) return handler_ret;
      return base_window<_ImplT, _TailT...>::propogate_message(hwnd, umsg, wparam, lparam, handled);
    }
  };

  template <typename _ImplT> struct base_window<_ImplT>{
    static const DWORD ExStyle = 0;
    static const DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP;

    virtual ~base_window(){ ::DestroyWindow(_handle); }

    base_window() : _handle(nullptr){}

    base_window(const base_window&) = delete;
    base_window& operator=(const base_window&) = delete;

    HWND operator*() const{ return _handle; }
    operator HWND() const{ return _handle; }

  protected:

    void create(HWND hParent){
      _handle = wtf::exception::throw_lasterr_if(
        ::CreateWindowEx(_ImplT::ExStyle, window_class_ex::get().name(), nullptr, _ImplT::Style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hParent, nullptr, reinterpret_cast<HINSTANCE>(&__ImageBase), this),
        [](HWND h){ return nullptr == h; });
    }

  private:
    template <typename, template <typename> typename ... > friend struct base_window;
    template <typename, template <typename> typename ... > friend struct window;

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled){ return 0; }

    virtual LRESULT propogate_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& handled){
      switch (umsg){
        case WM_CLOSE:
          DestroyWindow(hwnd); _handle = nullptr; break;
        case WM_DESTROY:
          PostQuitMessage(0); handled = true; break;
      }
      return 0;
    }

    static LRESULT CALLBACK window_proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
      try{
        typename _ImplT::window_type * pThis = nullptr;
        bool handled = false;
        LRESULT handler_ret;

        switch (umsg){
          case WM_NCCREATE:
          case WM_CREATE:
          {
            auto pCreate = reinterpret_cast<CREATESTRUCT*>(lparam);

            assert(pCreate);
            pThis = reinterpret_cast<typename _ImplT::window_type*>(pCreate->lpCreateParams);
            assert(pThis);
            pThis->_handle = hwnd;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            break;
          }
          default:
          {
            pThis = reinterpret_cast<typename _ImplT::window_type*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            break;
          }
        }

        if (!pThis) return DefWindowProc(hwnd, umsg, wparam, lparam);
        if (WM_ERASEBKGND == umsg){
          auto oDC = wtf::device_context::get_client(hwnd);

          handler_ret = pThis->propogate_message(hwnd, umsg, wparam, reinterpret_cast<LPARAM>(&oDC), handled);
        } else if (WM_PAINT == umsg){
          RECT r;
          if (0 == GetUpdateRect(hwnd, &r, FALSE)){
            return DefWindowProc(hwnd, umsg, wparam, lparam);
          }
          paint_struct oPaint(*pThis);
          auto oDC = wtf::device_context::get_client(hwnd);

          handler_ret = pThis->propogate_message(hwnd, umsg, reinterpret_cast<WPARAM>(&oDC), reinterpret_cast<LPARAM>(&oPaint), handled);
        } else{
          handler_ret = pThis->propogate_message(hwnd, umsg, wparam, lparam, handled);
        }

        if (handled) return handler_ret;
        return DefWindowProc(hwnd, umsg, wparam, lparam);
      }
      catch (const wtf::exception& ex){
        tstring sMsg = _T("");
        std::string swhat(ex.what());
        std::string scode(ex.code());
        std::copy(swhat.begin(), swhat.end(), std::back_inserter(sMsg));
        sMsg += _T("\n");
        std::copy(scode.begin(), scode.end(), std::back_inserter(sMsg));
        auto iRet = message_box::exec(hwnd, sMsg.c_str(), _T("An exception occurred."), message_box::buttons::cancel_retry_continue, message_box::icons::stop);
        if (message_box::response::cancel == iRet) abort();
        if (message_box::response::retry == iRet) return -1;
        throw;
      }
    }

    struct window_class_ex : WNDCLASSEX{
      static window_class_ex& get(){
        static window_class_ex _window_class_ex;
        return _window_class_ex;
      }

      LPCTSTR name(){ return _class_name.c_str(); }

      ~window_class_ex(){ UnregisterClass(_class_name.c_str(), reinterpret_cast<HINSTANCE>(&__ImageBase)); }

      window_class_ex(){
        //this goofy looking bit of code creates a unique class name in unicode or multibyte
        std::string sTemp = "wtf" + std::to_string(typeid(_ImplT).hash_code());
        for (auto ch : sTemp){
          _class_name.push_back(ch);
        }
        memset(this, 0, sizeof(WNDCLASSEX));
        cbSize = sizeof(WNDCLASSEX);
        style = CS_OWNDC | CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
        lpfnWndProc = &window_proc;
        cbClsExtra = sizeof(window<_ImplT>*);
        hInstance = reinterpret_cast<HINSTANCE>(&__ImageBase);
        lpszClassName = name();
        exception::throw_lasterr_if(RegisterClassEx(this), [](ATOM x){ return 0 == x; });
      }

      tstring _class_name;
    };

    HWND _handle;
  };
}
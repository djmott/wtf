#pragma once

namespace wtf{
  template <typename _ImplT, template <WNDPROC> typename _WindowClassT, template <typename> typename ... _PolicyListT> struct window;
  template <typename _ImplT, template <WNDPROC> typename _WindowClassT, template <typename> typename ... _PolicyListT> struct base_window;

  template <typename _ImplT, template <WNDPROC> typename _WindowClassT, template <typename> typename ... _PolicyListT>
  struct window : base_window<_ImplT, _WindowClassT, _PolicyListT...>{

    using _base_window_t = base_window<_ImplT, _WindowClassT, _PolicyListT...>;

    window(HWND hParent) : _base_window_t(){
      _handle = wtf::exception::throw_lasterr_if(
        ::CreateWindowEx(window_class_type::ex_window_style, window_class_type::get().name(), nullptr, window_class_type::window_style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hParent, nullptr, reinterpret_cast<HINSTANCE>(&__ImageBase), this),
        [](HWND h){ return nullptr == h; });
    }

    virtual ~window() = default;

  };



  template <typename _ImplT, template <WNDPROC> typename _WindowClassT, template <typename> typename _HeadT, template <typename> typename ... _TailT>
  struct base_window<_ImplT, _WindowClassT, _HeadT, _TailT...> : _HeadT<base_window<_ImplT,  _WindowClassT, _TailT...>>{
    using _super_t = _HeadT<base_window<_ImplT, _WindowClassT, _TailT...>>;
    using window_type = base_window<_ImplT, _WindowClassT, _HeadT, _TailT...>;

    base_window() = default;
    virtual ~base_window() = default;

  private:
    template <typename, template <WNDPROC> typename, template <typename> typename ... > friend struct base_window;

    virtual LRESULT propogate_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& handled) override{
      auto handler_ret = _super_t::handle_message(hwnd, umsg, wparam, lparam, handled);
      if (handled) return handler_ret;
      return base_window<_ImplT, _WindowClassT, _TailT...>::propogate_message(hwnd, umsg, wparam, lparam, handled);
    }

  };

  template <typename _ImplT, template <WNDPROC> typename _WindowClassT> struct base_window<_ImplT, _WindowClassT> {
    using window_type = base_window<_ImplT, _WindowClassT>;

    virtual ~base_window(){ ::DestroyWindow(_handle); }

    base_window() : _handle(nullptr){
      gdi_plus_initializer::get();
    }

    HWND native_handle() const{ return _handle; }
    HWND operator()() const{ return _handle; }
    operator HWND() const{ return _handle; }

  protected:

    virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wpara, LPARAM lparam, bool& bhandled){ return 0; }

  private:
    template <typename, template <WNDPROC> typename, template <typename> typename ... > friend struct base_window;
	  template <typename, template <WNDPROC> typename, template <typename> typename ... > friend struct window;


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
      typename _ImplT::window_type * pThis = nullptr;
      bool handled = false;

      switch (umsg){
        case WM_NCCREATE:
        case WM_CREATE:
        {
          auto pCreate = reinterpret_cast<CREATESTRUCT*>(lparam);
          assert(pCreate);
          pThis = reinterpret_cast<typename _ImplT::window_type*>(pCreate->lpCreateParams);
          pThis->_handle = hwnd;
          assert(pThis);
          SetWindowLongPtr(hwnd, 0, reinterpret_cast<LONG_PTR>(pThis));
          break;
        }
        default:
        {
          pThis = reinterpret_cast<typename _ImplT::window_type*>(GetWindowLongPtr(hwnd, 0));
          break;
        }
      }

      if (!pThis) return DefWindowProc(hwnd, umsg, wparam, lparam);

      auto handler_ret = pThis->propogate_message(hwnd, umsg, wparam, lparam, handled);
      if (handled) return handler_ret;
      return DefWindowProc(hwnd, umsg, wparam, lparam);
    }

    using window_class_type = _WindowClassT<&base_window::window_proc>;

    HWND _handle;

  };
}
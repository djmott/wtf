#pragma once
namespace wtf{

  namespace policy {
    template <typename> struct isa_tab_container;
  }

  template <typename _impl_t, template <typename> class..._policy_ts>
  class window_impl : public wtf::_::normalized_policies<_impl_t, _policy_ts...>::type{
    using __super_t = typename wtf::_::normalized_policies<_impl_t, _policy_ts...>::type;

  public:

    template <typename ... _ParamTs> window_impl(_ParamTs&&...oParam) : __super_t(std::forward<_ParamTs>(oParam)...){}

  protected:

    friend struct window;
    template <class, class> friend struct _::_;

    template <typename> friend struct policy::isa_tab_container;


    const std::type_info& type() const override{ return typeid(_impl_t); }


    int run() override{
      __super_t::_handle = wtf::exception::throw_lasterr_if(
        ::CreateWindowEx(_impl_t::ExStyle, window_class_type::get().name(), nullptr, _impl_t::Style,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (__super_t::_parent ? __super_t::_parent->_handle : nullptr),
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
        _impl_t * pThis = nullptr;

        if (WM_NCCREATE == umsg){
          auto pCreate = reinterpret_cast<CREATESTRUCT*>(lparam);
          assert(pCreate);
          pThis = reinterpret_cast<_impl_t*>(pCreate->lpCreateParams);
          assert(pThis);
          pThis->_handle = hwnd;
          SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
          for (auto pChild : pThis->children()){
            pChild->run();
          }
        } else{
          pThis = reinterpret_cast<_impl_t*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (!pThis) return DefWindowProc(hwnd, umsg, wparam, lparam);

        wtf::window_message msg{ hwnd, umsg, wparam, lparam, false, 0 };

        if (WM_ERASEBKGND == umsg){
          auto oDC = wtf::_::device_context::get_client(hwnd);
          msg.lparam = reinterpret_cast<LPARAM>(&oDC);
          pThis->fwd_msg(msg);
          return msg.lresult;
        } else if (WM_PAINT == umsg){
          RECT r;
          if (0 == GetUpdateRect(hwnd, &r, FALSE)){
            return DefWindowProc(hwnd, umsg, wparam, lparam);
          }
          wtf::_::paint_struct oPaint(*pThis);
          auto oDC = wtf::_::device_context::get_client(hwnd);
          msg.wparam = reinterpret_cast<WPARAM>(&oDC);
          msg.lparam = reinterpret_cast<LPARAM>(&oPaint);
          pThis->fwd_msg(msg);
          return msg.lresult;
        } else{
          pThis->fwd_msg(msg);
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

    using window_class_type = wtf::_::window_class_ex<_impl_t, &window_impl<_impl_t, _policy_ts...>::window_proc>;
  };

}

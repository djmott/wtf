#pragma once

namespace wtf{
  namespace _{
    //everything in the hidden _ namespace is for internal use by the library

    template <typename, template <typename> class ...> struct base_window;

    // is_same_policy meta-function returns true if two policies are the same type
    template <template <typename> class _Policy1, template <typename> class _Policy2> struct is_same_policy : std::false_type{};

    template <template <typename> class _Policy1> struct is_same_policy<_Policy1, _Policy1> : std::true_type{};


    // has_policy compile time meta-function returns true if a concrete implementation inherits from a policy
    template <template <typename> class _DeclT, template <typename> class ... _TailT> struct has_policy;

    template <template <typename> class _DeclT>
    struct has_policy<_DeclT>{ static const bool value = false; };


    template <template <typename> class _DeclT, template <typename> class _HeadT, template <typename> class ... _TailT>
    struct has_policy<_DeclT, _HeadT, _TailT...>{
      static const bool value = is_same_policy<_DeclT, _HeadT>::value ? true : has_policy<_DeclT, _TailT...>::value;
    };


    // concrete_impl compile time meta-function returns the fully defined concrete implementation of an inherited policy
    template <template <typename> class _TargetPolicyT, typename _ImplT, template <typename> class ... _PolicyListT>
    struct concrete_impl;

    template <template <typename> class _TargetPolicyT, typename _ImplT>
    struct concrete_impl<_TargetPolicyT, _ImplT> : std::false_type{};

    template <template <typename> class _TargetT, typename _ImplT, template <typename> class _HeadT, template <typename> class ... _TailT>
    struct concrete_impl<_TargetT, _ImplT, _HeadT, _TailT...>{
      using __is_target_policy_t = is_same_policy<_TargetT, _HeadT>;
      using __is_target_return_t = base_window<_ImplT, _TargetT, _TailT...>;
      using __is_other_return_t = typename concrete_impl<_TargetT, _ImplT, _TailT...>::type;
      using type = typename std::conditional<__is_target_policy_t::value, __is_target_return_t, __is_other_return_t>::type;
    };


    template <typename, typename> struct policy_list_concat;
    template <template<typename> class ... _list1, template<typename> class ... _list2>
    struct policy_list_concat<policy::list<_list1...>, policy::list<_list2...>>{
      using type = policy::list<_list1..., _list2...>;
    };


    //removes all occurances of Target in policy list
    template <template <typename> class _TargetT, typename _PolicyListT>
    struct remove_policy_from;

    template <template <typename> class _TargetT>
    struct remove_policy_from<_TargetT, policy::list<>>{ using type = policy::list<>; };


    template <template <typename> class _TargetT, template <typename> class _HeadT, template <typename> class ... _TailT>
    struct remove_policy_from<_TargetT, policy::list<_HeadT, _TailT...>>{
      static const bool __SkipIt = is_same_policy<_TargetT, _HeadT>::value;
      using head_list_t = typename std::conditional<__SkipIt, policy::list<>, policy::list<_HeadT>>::type;
      using tail_list_t = typename remove_policy_from<_TargetT, policy::list<_TailT...>>::type;
      using type = typename policy_list_concat<head_list_t, tail_list_t>::type;
    };

    //removes duplicates from a policy list
    template <typename> struct remove_duplicate_policies;

    template <> struct remove_duplicate_policies<policy::list<>>{ using type = policy::list<>; };

    template <template <typename> class _HeadT, template <typename> class ... _TailT>
    struct remove_duplicate_policies<policy::list<_HeadT, _TailT...>>{
      using __no_head_list = typename remove_policy_from<_HeadT, policy::list<_TailT...>>::type;
      using unique_tails = typename remove_duplicate_policies<__no_head_list>::type;
      using type = typename policy_list_concat<policy::list<_HeadT>, unique_tails>::type;
    };


    //recursively traverses the policy requirements and appends them to the the list
    template <typename> struct requirement_collector;

    template <> struct requirement_collector<policy::list<>>{ using type = policy::list<>; };

    template <template <typename> class _HeadT, template <typename> class ..._TailT>
    struct requirement_collector<policy::list<_HeadT, _TailT...>>{
      using head_requirements = typename policy::traits<_HeadT>::requirements;
      using new_tail = typename policy_list_concat<policy::list<_TailT...>, head_requirements>::type;
      using unique_tail = typename remove_duplicate_policies<new_tail>::type;
      using tail_requirements = typename requirement_collector<unique_tail>::type;
      using unique_tail_requirements = typename remove_duplicate_policies<tail_requirements>::type;
      using full_policy_list = typename policy_list_concat<policy::list<_HeadT>, unique_tail_requirements>::type;
      using type = typename remove_duplicate_policies<full_policy_list>::type;
    };

    template <typename _ImplT, typename _ListT> struct policy_list_to_base_win;
    template <typename _ImplT, template <typename> class ... _PolicyListT>
    struct policy_list_to_base_win<_ImplT, policy::list<_PolicyListT...>>{
      using type = base_window<_ImplT, _PolicyListT...>;
      using window_type = window<_ImplT, _PolicyListT...>;
    };

    template <typename _ImplT, template <typename> class ... _PolicyListT>
    struct window_impl{
      using policies_with_deps = typename requirement_collector<policy::list<_PolicyListT...>>::type;
      using type = typename policy_list_to_base_win<_ImplT, policies_with_deps>::type;
      using window_type = typename policy_list_to_base_win<_ImplT, policies_with_deps>::window_type;
    };


    /** base_window
      * constructs the inheritance chain of policies
      */
    template <typename _ImplT, template <typename> class _HeadT, template <typename> class ... _TailT>
    struct base_window<_ImplT, _HeadT, _TailT...> : _HeadT<base_window<_ImplT, _TailT...>>{
      using _super_t = _HeadT<base_window<_ImplT, _TailT...>>;
      using window_type = base_window<_ImplT, _HeadT, _TailT...>;

      template <template <typename> class _PolicyT>
      using concrete_policy_type =  _::concrete_impl<_PolicyT, _ImplT, _HeadT, _TailT...>;


      base_window() = default;
      virtual ~base_window() = default;

      base_window(const base_window&) = delete;
      base_window &operator=(const base_window &) = delete;

      /// returns true if the class inherits from the specified policy
      template <template <typename> class _PolicyT> bool has_policy() const{ 
        return _::has_policy<_PolicyT, _HeadT, _TailT...>::value; 
      }

      /// returns a reference to the concrete policy implementation of this instance
      template <template <typename> class _PolicyT> typename concrete_policy_type<_PolicyT>::type& get_policy(){
        return *static_cast<typename concrete_policy_type<_PolicyT>::type*>(this);
      }

      /// same as above but const
      template <template <typename> class _PolicyT> const typename concrete_policy_type<_PolicyT>::type& get_policy() const{
        return *static_cast<const typename concrete_policy_type<_PolicyT>::type*>(this);
      }

    private:
      template <typename, template <typename> class ... > friend struct base_window;

      virtual LRESULT propagate_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& handled) override{
        auto handler_ret = _super_t::handle_message(hwnd, umsg, wparam, lparam, handled);
        if (handled) return handler_ret;
        return base_window<_ImplT, _TailT...>::propagate_message(hwnd, umsg, wparam, lparam, handled);
      }
    };


    /** Specialization that terminates the inheritance hierarchy
      * super most base class of all windows
     */
    template <typename _ImplT> struct base_window<_ImplT>{
      /// an implementation may want to use different window styles so add their definitions as class wide static constants
      static const DWORD ExStyle = 0;
      static const DWORD Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP;

      virtual ~base_window(){ if (_handle) ::DestroyWindow(_handle); }

      base_window() : _handle(nullptr){}

      base_window(const base_window&) = delete;
      base_window& operator=(const base_window&) = delete;

      HWND operator*() const{ return _handle; }
      operator HWND() const{ return _handle; }


    protected:

      void create(HWND hParent){
        _handle = wtf::exception::throw_lasterr_if(
          ::CreateWindowEx(_ImplT::ExStyle, window_class_ex<_ImplT, &base_window<_ImplT>::window_proc>::get().name(), nullptr, _ImplT::Style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hParent, nullptr, instance_handle(), this),
          [](HWND h){ return nullptr == h; });
      }

    private:
      template <typename, template <typename> class ... > friend struct base_window;
      template <typename, template <typename> class ... > friend struct window;

      virtual LRESULT handle_message(HWND , UINT , WPARAM , LPARAM , bool& ){ return 0; }

      virtual LRESULT propagate_message(HWND hwnd, UINT umsg, WPARAM , LPARAM , bool& handled){
        switch (umsg){
          case WM_CLOSE:
            DestroyWindow(hwnd); _handle = nullptr; break;
          case WM_DESTROY:
            PostQuitMessage(0); handled = true; break;
        }
        return 0;
      }

      static LRESULT CALLBACK window_proc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        std::string sTemp = typeid(_ImplT).name();
        sTemp += " ";
        sTemp += msg_name(umsg);
        sTemp += "\n";
        OutputDebugStringA(sTemp.c_str());
        try{
          _ImplT * pThis = nullptr;
          bool handled = false;
          LRESULT handler_ret;

          switch (umsg){
            case WM_NCCREATE:
            case WM_CREATE:
            {
              auto pCreate = reinterpret_cast<CREATESTRUCT*>(lparam);

              assert(pCreate);
              pThis = reinterpret_cast<_ImplT*>(pCreate->lpCreateParams);
              assert(pThis);
              pThis->_handle = hwnd;
              SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
              break;
            }
            default:
            {
              pThis = reinterpret_cast<_ImplT*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
              break;
            }
          }

          if (!pThis) return DefWindowProc(hwnd, umsg, wparam, lparam);
          if (WM_ERASEBKGND == umsg){
            auto oDC = wtf::device_context::get_client(hwnd);

            //try implementation before propagating
            handler_ret = pThis->try_impl_handle_message(hwnd, umsg, wparam, reinterpret_cast<LPARAM>(&oDC), handled);
            if (!handled) handler_ret = pThis->propagate_message(hwnd, umsg, wparam, reinterpret_cast<LPARAM>(&oDC), handled);

          } else if (WM_PAINT == umsg){
            RECT r;
            if (0 == GetUpdateRect(hwnd, &r, FALSE)){
              return DefWindowProc(hwnd, umsg, wparam, lparam);
            }
            paint_struct oPaint(*pThis);
            auto oDC = wtf::device_context::get_client(hwnd);

            handler_ret = pThis->try_impl_handle_message(hwnd, umsg, reinterpret_cast<WPARAM>(&oDC), reinterpret_cast<LPARAM>(&oPaint), handled);
            if (!handled) handler_ret = pThis->propagate_message(hwnd, umsg, reinterpret_cast<WPARAM>(&oDC), reinterpret_cast<LPARAM>(&oPaint), handled);

          } else{
            handler_ret = pThis->try_impl_handle_message(hwnd, umsg, wparam, lparam, handled);
            if (!handled) handler_ret = pThis->propagate_message(hwnd, umsg, wparam, lparam, handled);
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

      HWND _handle;
  };
  }
}
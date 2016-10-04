#pragma once

namespace wtf{


  template <typename _ImplT, template <typename> typename ... _PolicyListT>
  struct window :  _::window_impl<_ImplT, _PolicyListT...>::type {

    using _base_window_t = typename  _::window_impl<_ImplT, _PolicyListT...>::type;
    using window_type = typename  _::window_impl<_ImplT, _PolicyListT...>::window_type;


    window(HWND hParent, bool bCreate = true) : _base_window_t(){
      //its neccessary for all the constructors in the inheritance chain to initialize before creating the window
      //otherwise the vtbl wont point to intended overloads
      if (bCreate) window::create(hParent);
    }

    virtual ~window() = default;

  };

}

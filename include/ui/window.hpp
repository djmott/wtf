#pragma once

namespace wtf{


  template <typename _ImplT, template <typename, typename> class ... _PolicyListT>
  struct window :  _::base_window<_ImplT, _PolicyListT...> {

    using _base_window_t = typename  _::base_window<_ImplT, _PolicyListT...>;
    using window_type = typename  _::base_window<_ImplT, _PolicyListT...>::window_type;

    explicit window(iwindow * parent) : _base_window_t(parent){ }
    virtual ~window() = default;

  protected:
    LRESULT handle_message(HWND , UINT , WPARAM , LPARAM , bool& ){ return 0; }

  };

}

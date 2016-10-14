#pragma once

namespace wtf{


  template <typename _ImplT, template <typename, typename> class ... _PolicyListT>
  struct window :  _::window_impl<_ImplT, _PolicyListT...>::type {

    using _base_window_t = typename  _::window_impl<_ImplT, _PolicyListT...>::type;
    using window_type = typename  _::window_impl<_ImplT, _PolicyListT...>::window_type;


    window(HWND hParent, bool bCreate = true) : _base_window_t(){
      //its neccessary for all the constructors in the inheritance chain to initialize before creating the window
      //otherwise the vtbl wont point to intended virtual overrides
      if (bCreate) window::create(hParent);
    }

    virtual ~window() = default;
    window(const window&) = delete;
    window &operator=(const window &) = delete;
    window(window&&) = delete;
    window &operator=(window&&) = delete;

  protected:
    virtual LRESULT handle_message(HWND , UINT , WPARAM , LPARAM , bool& ){ return 0; }
  private:
    template <typename, template <typename,typename> class ... > friend struct _::base_window;

    /* 
    here messages are received from the main message pump and they're just fowarded to handle_message
    this permits subclasses which have overridden the handle_message method to capture the message and deal with it
    */
    LRESULT try_impl_handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled){ 
      return handle_message(hwnd, umsg, wparam, lparam, bhandled);
    }

  };

}

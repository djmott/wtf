#pragma once
namespace wtf{

  namespace _{
    template <WNDPROC _wp> using form_class = window_class_ex<_wp, WS_EX_OVERLAPPEDWINDOW, WS_OVERLAPPED, CS_OWNDC | CS_HREDRAW | CS_VREDRAW, icons::application, cursors::arrow, create_window_system_brush<system_colors::button_face>, menus::null_menu, icons::application>;
  }

  template <typename _ImplT, template <typename> typename ... _PolicyListT> 
  struct form : window<_ImplT, _::form_class, has_move, has_close, has_show, has_titlebar, _PolicyListT...>
  {};

}
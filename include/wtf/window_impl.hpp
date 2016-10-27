#pragma once

namespace wtf{

  template <typename _ImplT, policy..._Policies> class window_impl : public window<_ImplT, _Policies...>{
    using __super_t = window<_ImplT, _Policies...>;
    template <typename, policy ... > friend class window_impl;
    template <typename, policy ... > friend class window;

    virtual void handle_msg(window_message& msg) = 0;

    LRESULT propagate_msg(window_message& msg){
      __super_t::handle_msg(msg);
      return __super_t::propagate_msg(msg);
    }


  public:
    template <typename ... _ParamTs> window_impl(_ParamTs&&...oParam) : __super_t(std::forward<_ParamTs>(oParam)...){}
  };


}
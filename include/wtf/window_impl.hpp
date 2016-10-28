#pragma once

namespace wtf{

  template <class _ImplT, template <class,class> class..._Policies> class window_impl;


  template <class _ImplT, template <class, class> class _HeadT, template <class, class> class..._TailT>
  class window_impl<_ImplT, _HeadT, _TailT...> : _HeadT<_ImplT, window_impl<_ImplT, _TailT...>>{

    virtual void handle_msg(window_message& msg) {}

    LRESULT propagate_msg(window_message& msg){
      __super_t::handle_msg(msg);
      return __super_t::propagate_msg(msg);
    }


  public:
    template <typename ... _ParamTs> window_impl(_ParamTs&&...oParam) : __super_t(std::forward<_ParamTs>(oParam)...){}

  };


  template <class _ImplT>
  class window_impl<_ImplT> : window<_ImplT>{
    virtual void handle_msg(window_message& msg) = 0;

    LRESULT propagate_msg(window_message& msg){
      __super_t::handle_msg(msg);
      return __super_t::propagate_msg(msg);
    }


  public:
    template <typename ... _ParamTs> window_impl(_ParamTs&&...oParam) : window(std::forward<_ParamTs>(oParam)...){}

  };



}
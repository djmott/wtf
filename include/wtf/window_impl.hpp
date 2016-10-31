#pragma once

namespace wtf{

  template <typename _ImplT, template <typename, typename> typename..._Policies> class window_impl : public normalized_policies<_ImplT, _Policies...>::type{
    using __super_t = typename normalized_policies<_ImplT, _Policies...>::type;
  public:
    template <typename ... _ParamTs> window_impl(_ParamTs&&...oParam) : __super_t(std::forward<_ParamTs>(oParam)...){}
  protected:
    friend class window<_ImplT>;

    void handle_msg(window_message& msg) override{
      __super_t::handle_msg(msg);
    }


  };

}
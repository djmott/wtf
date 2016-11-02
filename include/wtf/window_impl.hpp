/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  template <typename _ImplT, template <typename> typename..._Policies> 
  class window_impl : public normalized_policies<_ImplT, _Policies...>::type{
    using __super_t = typename normalized_policies<_ImplT, _Policies...>::type;
  public:
    template <typename ... _ParamTs> window_impl(_ParamTs&&...oParam) : __super_t(std::forward<_ParamTs>(oParam)...){}
  protected:
    friend struct window<_ImplT>;

    void handle_msg(_::window_message& msg) override{
      __super_t::handle_msg(msg);
    }


  };

}
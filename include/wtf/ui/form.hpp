/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct isa_form : _SuperT{

      static const DWORD ExStyle = WS_EX_OVERLAPPEDWINDOW;
      static const DWORD Style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

      explicit isa_form(iwindow * pParent) : _SuperT(pParent){}

      isa_form() : isa_form(nullptr){}

      void show() override{
        if (!_SuperT::_handle) _SuperT::exec();
        _SuperT::show();
      }

      int exec() override{
        _::message oMsg;
        _SuperT::exec();
        auto iRet = oMsg.pump();
        return iRet;
      }


      int top() const override{
        return rect<coord_frame::screen>::get(*this).top;
      }

      int left() const  override{
        return rect<coord_frame::screen>::get(*this).left;
      }

      int width() const  override{
        auto r = rect<coord_frame::client>::get(*this);
        return r.right - r.left;
      }

      int height() const  override{
        auto r = rect<coord_frame::client>::get(*this);
        return r.bottom - r.top;
      }

    protected:

      void on_wm_destroy() override{
        bool bQuit = false;
        {
          std::unique_lock<std::mutex> oLock(wtf::_::_active_forms_lock());
          auto & oForms = wtf::_::_active_forms();
          auto it = std::find(oForms.begin(), oForms.end(), this);
          if (oForms.end() != it){
            oForms.erase(it);
            bQuit = (0 == oForms.size());
          }
        }
        if (bQuit) ::PostQuitMessage(0);
        _SuperT::on_wm_destroy();
      }

      void on_wm_create() override{
        std::unique_lock<std::mutex> oLock(wtf::_::_active_forms_lock());
        wtf::_::_active_forms().push_back(this);
        _SuperT::on_wm_create();
      }

    };
  }

  template <> struct policy_traits<policy::isa_form>{
    using requires = policy_list<policy::has_background, policy::has_cursor, policy::has_icon,
    policy::has_titlebar, policy::has_size, policy::has_move, policy::has_show,
    policy::wm_destroy, policy::wm_create, policy::wm_size, policy::wm_close,
    policy::wm_showwindow, policy::wm_activate>;
  };

  struct form : window_impl<form, policy::isa_form>{
    explicit form(iwindow * pParent) : window_impl(pParent){}
    form() : form(nullptr){}
  };

}

#pragma once

namespace wtf{
  namespace policy{
    template <typename _ImplT, typename _SuperT>
    class isa_form : public _SuperT{

      
    public:

      explicit isa_form(iwindow * pParent) : _SuperT(pParent){}

      isa_form() : isa_form(nullptr){}

      virtual void show() override{
        if (!_handle) _SuperT::exec();
        _SuperT::show();
      }

      virtual int exec() override{
        message oMsg;
        _SuperT::exec();
        auto iRet = oMsg.pump();
        return iRet;
      }

      virtual int top() const override{
        return rect<coord_frame::screen>::get(*this).top;
      }

      virtual int left() const  override{
        return rect<coord_frame::screen>::get(*this).left;
      }

      virtual int width() const  override{
        auto r = rect<coord_frame::client>::get(*this);
        return r.right - r.left;
      }

      virtual int height() const  override{
        auto r = rect<coord_frame::client>::get(*this);
        return r.bottom - r.top;
      }

    protected:
      virtual void handle_msg(window_message& msg) override{}

      virtual void on_wm_destroy() override{
        bool bQuit = false;
        {
          std::unique_lock<std::mutex> oLock(_::_active_forms_lock());
          auto & oForms = _::_active_forms();
          auto it = std::find(oForms.begin(), oForms.end(), this);
          if (oForms.end() != it){
            oForms.erase(it);
            bQuit = (0 == oForms.size());
          }
        }
        if (bQuit) ::PostQuitMessage(0);
        _SuperT::on_wm_destroy();
      }

      virtual void on_wm_create() override{
        std::unique_lock<std::mutex> oLock(_::_active_forms_lock());
        _::_active_forms().push_back(this);
        _SuperT::on_wm_create();
      }

    };
  }
  struct form : window_impl<form, policy::isa_form>{
    static const DWORD ExStyle = WS_EX_OVERLAPPEDWINDOW;
    static const DWORD Style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    explicit form(iwindow * pParent) : window_impl(pParent){}
    form() : form(nullptr){}

  };

}

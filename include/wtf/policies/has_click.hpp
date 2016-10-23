#pragma once


namespace wtf {
  namespace policy {
    /** has_click
    * produces click events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_click : _SuperT {

      using mouse_msg_param = messages::mouse_msg_param;

    protected:

      virtual void on_wm_click(const mouse_msg_param& )=0{}

      virtual LRESULT on_wm_mouse_down(const messages::mouse_msg_param& oParam, bool &) override{
        _Down = oParam.button;
        return 0; 
      }

      virtual LRESULT on_wm_mouse_up(const mouse_msg_param& oParam, bool&) override{
        if (rect<coord_frame::client>::get(*this).is_in(oParam.position) && _Down == oParam.button){
          on_wm_click(oParam);
          _Down = mouse_msg_param::buttons::unspecified;
        }
        return 0;
      }


      explicit has_click(window<void,void> * pParent) : _SuperT(pParent){}

    private:
       mouse_msg_param::buttons _Down = mouse_msg_param::buttons::unspecified;
    };
  }
}

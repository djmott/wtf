#pragma once


namespace wtf {
  namespace policy {
    /** has_click
    * produces click events
    */
    template<typename _SuperT, typename _ImplT>
    struct has_click : _SuperT::window_type::template add_policy<messages::wm_mouse_down, messages::wm_mouse_up> {

      using mouse_msg_param = messages::mouse_msg_param;

    protected:

      using _super_t = typename _SuperT::window_type::template add_policy<messages::wm_mouse_down, messages::wm_mouse_up>;

      virtual void wm_click(const mouse_msg_param& m) {}

      virtual LRESULT on_wm_mouse_down(const messages::mouse_msg_param& oParam, bool &) override{
        _Down = oParam.button;
        return 0; 
      }

      virtual LRESULT on_wm_mouse_up(const mouse_msg_param& oParam, bool&) override{
        if (rect<coord_frame::client>::get(*this).is_in(oParam.position) && _Down == oParam.button){
          wm_click(oParam);
          _Down = mouse_msg_param::buttons::unspecified;
        }
        return 0;
      }


      has_click(window<void> * pParent) : _super_t(pParent){}

    private:
       mouse_msg_param::buttons _Down = mouse_msg_param::buttons::unspecified;
    };
  }
}

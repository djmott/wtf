#pragma once

namespace wtf {
  namespace policy {
    /** has_caret
    * Controls the caret of text/input elements
    */
    template<typename _SuperT, typename>
    struct has_caret : _SuperT::window_type::template add_policy<messages::wm_setfocus, messages::wm_killfocus> {

      virtual int caret_width() const{ return _width; }
      virtual void caret_width(int newval){ _width = newval; }

      virtual int caret_height() const{ return _height; }
      virtual void caret_height(int newval){ _height = newval; }

      virtual void create_caret() {
        wtf::exception::throw_lasterr_if(::CreateCaret(*this, nullptr, _width, _height), [](BOOL b) { return !b; });
        _caret_visible = false;
      }

      virtual void destroy_caret() {
        wtf::exception::throw_lasterr_if(::DestroyCaret(), [](BOOL b) { return !b; });
        _caret_visible = false;
      }

      virtual bool caret_visible() const{ return _caret_visible; }
      virtual void caret_visible(bool newval) {
        if (_caret_visible){
          if (newval) return;
          wtf::exception::throw_lasterr_if(::HideCaret(*this), [](BOOL b){ return !b; });
        }else{
          if (!newval) return;
          wtf::exception::throw_lasterr_if(::ShowCaret(*this), [](BOOL b){ return !b; });
        }
        _caret_visible = newval;
      }

      virtual UINT caret_blink_rate() const { return _blink_rate; }

      virtual void caret_blink_rate(UINT newval) {
        _blink_rate = newval;
        wtf::exception::throw_lasterr_if(::SetCaretBlinkTime(newval), [](BOOL b) { return !b; });
      }

      virtual void caret_position(const point<coord_frame::client> &pos) {
        _pos = pos;
        wtf::exception::throw_lasterr_if(::SetCaretPos(_pos.x, _pos.y), [](BOOL b) { return !b; });
      }

      virtual point<coord_frame::client> caret_position() const{ return _pos; }

    protected:

      using _super_t = typename _SuperT::window_type::template add_policy<messages::wm_setfocus, messages::wm_killfocus>;

      has_caret(window<void> * pParent) : _super_t(pParent){}

      virtual LRESULT on_wm_setfocus(HWND hwnd, bool& bHandled) override{
        create_caret();
        caret_position(_pos);
        caret_visible(true);
        caret_blink_rate(_blink_rate);
        return _super_t::on_wm_setfocus(hwnd, bHandled);
      }

      virtual LRESULT on_wm_killfocus(HWND hwnd, bool& bHandled) override{
        destroy_caret();
        return _super_t::on_wm_killfocus(hwnd, bHandled);
      }

    private:
      UINT _blink_rate = 250;
      point<coord_frame::client> _pos = point<coord_frame::client>(0,0);
      bool _caret_visible = false;
      int _width = 1;
      int _height = 1;
    };
  }
}

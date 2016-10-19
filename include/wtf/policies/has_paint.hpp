#pragma once

namespace wtf {
  namespace policy {
    /** has_paint
    * adds members and events related to painting
    */
    template<typename _SuperT, typename _ImplT>
    struct has_paint : _SuperT {

      void update() const {
        wtf::exception::throw_lasterr_if(::UpdateWindow(*this), [](BOOL b) { return !b; });
      }

      void refresh(bool erase = true) const {
        wtf::exception::throw_lasterr_if(::InvalidateRect(*this, nullptr, erase ? TRUE : FALSE),
                                         [](BOOL b) { return !b; });
      }

      void lock_update() const{ ::LockWindowUpdate(*this); }

      void unlock_update() const{ ::LockWindowUpdate(nullptr); }

      virtual const brush &background_brush() const { return _background_brush; }

      void background_brush(brush &&newval) { _background_brush.swap(newval); }



    protected:
      virtual void wm_paint(const device_context&, const paint_struct&) {}
      virtual void wm_erase_background(const device_context& ctx, const rect<coord_frame::client>& client, bool&) {}

      has_paint(window<void> * pParent) : _SuperT(pParent){}


      LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool &bhandled) {
        if (WM_PAINT == umsg) {
          wm_paint(*reinterpret_cast<const device_context *>(wparam),
                     *reinterpret_cast<const paint_struct *>(lparam));
        } else if (WM_ERASEBKGND == umsg) {
          auto &oDC = *reinterpret_cast<const device_context *>(lparam);
          wm_erase_background(oDC, rect<coord_frame::client>::get(*this), bhandled);
          if (!bhandled){
            bhandled = true;
            oDC.fill(rect<coord_frame::client>::get(*this), _background_brush);
          }
          return 1;
        }
        return 0;
      }

      brush _background_brush = brush::system_brush(system_colors::button_face);

    };

  }
}

#pragma once

namespace wtf {
  namespace policy {
    /** has_paint
    * adds members and events related to painting
    */
    template<typename _SuperT>
    struct has_paint : _SuperT {

      has_paint() : _SuperT(), _background_brush(brush::system_brush(system_colors::button_face)) {}

      wtf::callback<void(const device_context &, const paint_struct&)> PaintEvent;

      void update() const {
        wtf::exception::throw_lasterr_if(::UpdateWindow(*this), [](BOOL b) { return !b; });
      }

      void refresh(bool erase = true) const {
        auto area = rect::get_client_rect(*this);
        wtf::exception::throw_lasterr_if(::InvalidateRect(*this, &area, erase ? TRUE : FALSE),
                                         [](BOOL b) { return !b; });
      }

      virtual const brush &background_brush() const { return _background_brush; }

      void background_brush(brush &&newval) { _background_brush.swap(newval); }

    protected:
      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool &bhandled) override {
        if (WM_PAINT == umsg) {
          PaintEvent(*reinterpret_cast<const device_context *>(wparam),
                     *reinterpret_cast<const paint_struct *>(lparam));
        } else if (WM_ERASEBKGND == umsg) {
          auto &oDC = *reinterpret_cast<const device_context *>(lparam);
          auto oRect = rect::get_client_rect(*this);
          oDC.fill(oRect, background_brush());
          bhandled = true;
          return 1;
        }
        return 0;
      }

      brush _background_brush;

    };

  }
}

#pragma once

namespace wtf {
  namespace policy {
    /** has_paint
    * adds members and events related to painting
    */
    template<typename _SuperT>
    struct has_paint : _SuperT {

      has_paint() : _SuperT(), _background_brush(brush::system_brush(system_colors::button_face)) {}
      has_paint(const has_paint&) = delete;
      has_paint &operator=(const has_paint &) = delete;
      has_paint(has_paint&&) = delete;
      has_paint &operator=(has_paint &&) = delete;

      void update() const {
        wtf::exception::throw_lasterr_if(::UpdateWindow(*this), [](BOOL b) { return !b; });
      }

      void refresh(bool erase = true) const {
        auto area = rect::get_client_rect(*this);
        wtf::exception::throw_lasterr_if(::InvalidateRect(*this, &area, erase ? TRUE : FALSE),
                                         [](BOOL b) { return !b; });
      }

      void lock_update() const{ ::LockWindowUpdate(*this); }
      void unlock_update() const{ ::LockWindowUpdate(nullptr); }

      virtual const brush &background_brush() const { return _background_brush; }

      void background_brush(brush &&newval) { _background_brush.swap(newval); }

    protected:

      virtual void PaintEvent(const device_context&, const paint_struct&){}
      virtual void EraseBackgroundEvent(const device_context& ctx, const rect& client, bool& handled ){
        ctx.fill(client, background_brush());
      }

      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM wparam, LPARAM lparam, bool &bhandled) override {
        if (WM_PAINT == umsg) {
          PaintEvent(*reinterpret_cast<const device_context *>(wparam),
                     *reinterpret_cast<const paint_struct *>(lparam));
        } else if (WM_ERASEBKGND == umsg) {
          auto &oDC = *reinterpret_cast<const device_context *>(lparam);
          EraseBackgroundEvent(oDC, rect::get_client_rect(*this), bhandled);
          if (bhandled) return 1;
        }
        return 0;
      }

      brush _background_brush;

    };

  }
}

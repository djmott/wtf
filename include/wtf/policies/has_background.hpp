#pragma once

namespace wtf{

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_background, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
    public:

      const brush& background_brush() const{ return _background_brush; }
      void background_brush(brush&& newval){ _background_brush = std::move(newval); }

    protected:

      LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
        if (WM_ERASEBKGND == umsg){
          auto &dc = *reinterpret_cast<const device_context *>(lparam);
          dc.fill(rect<coord_frame::client>::get(*this), background_brush());
          return TRUE;
        }
        return __super_t::handle_message(hwnd, umsg, wparam, lparam);
      }

      explicit window(iwindow * pParent) : __super_t(pParent){}

    private:
      brush _background_brush = brush::solid_brush(system_colors::button_face);
    };
}

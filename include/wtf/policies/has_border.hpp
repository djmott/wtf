#pragma once

namespace wtf {

  enum class border_styles{
    none = 0,
    flat,
    raised,
    lowered,
    bumped,
    etched,
    double_raised,
    double_lowered,
  };

    /** has_border
    * Creates borders
    */
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_border, _Policies...> : public window<_ImplT, policy::wm_nccalcsize, policy::wm_ncpaint, _Policies...>{
      using __super_t = window<_ImplT, policy::wm_nccalcsize, policy::wm_ncpaint, _Policies...>;
      template <typename, policy ... > friend class window;
    public:


      virtual int border_width() const{
        switch (_border_style){
          case border_styles::none: return 0;
          case border_styles::flat:
          case border_styles::raised:
          case border_styles::lowered: return 1;
          default: return 2;
        }
      }
      virtual const rgb& border_highlight() const{ return _border_highlight; }
      virtual void border_highlight(const rgb& newval){ 
        _border_highlight = newval; 
        refresh_border();
      }

      virtual const rgb& border_shadow() const{ return _border_shadow; }
      virtual void border_shadow(const rgb& newval){ 
        _border_shadow = newval;
        refresh_border();
      }

      virtual border_styles border_style() const{ return _border_style; }
      virtual void border_style(border_styles newval){ 
        _border_style = newval; 
        refresh_border();
      }

      virtual void enable_border_elements(bool top, bool right, bool bottom, bool left){
        _draw_top = top; _draw_left = left; _draw_right = right; _draw_bottom = bottom;
      }

    protected:

      explicit window(iwindow * pParent) : __super_t(pParent){}

      void refresh_border(){
        if (!_handle) return;
        wtf::exception::throw_lasterr_if(
          ::RedrawWindow(*this, nullptr, nullptr, RDW_FRAME | RDW_INVALIDATE | RDW_NOCHILDREN),
          [](BOOL b){ return !b; }
        );
      }

      virtual void on_wm_ncpaint(const device_context& dc, const rect<coord_frame::client>& oClient) override{

        auto highlight = pen::create(pen::style::solid, 1, border_highlight());
        auto shadow = pen::create(pen::style::solid, 1, border_shadow());

        rect<coord_frame::client> client = oClient;
        client.bottom--;
        client.right--;

        //draw outer border
        switch (border_style()){
          case border_styles::none:
            return;
          case border_styles::flat:
            if (_draw_right) dc.line(shadow, client.right, client.top, client.right, 1 + client.bottom);
            if (_draw_bottom) dc.line(shadow, client.left, client.bottom, client.right, client.bottom);
            if (_draw_top) dc.line(shadow, client.left, client.top, client.right, client.top);
            if (_draw_left) dc.line(shadow, client.left, client.top, client.left, client.bottom);
            return;
          case border_styles::etched:
          case border_styles::lowered:
          case border_styles::double_lowered:
            std::swap(highlight, shadow); //fall through
          case border_styles::bumped:
          case border_styles::raised:
          case border_styles::double_raised:
            if (_draw_right) dc.line(shadow, client.right, client.top, client.right, 1 + client.bottom);
            if (_draw_bottom) dc.line(shadow, client.left, client.bottom, client.right, client.bottom);
            if (_draw_top) dc.line(highlight, client.left, client.top, client.right, client.top);
            if (_draw_left) dc.line(highlight, client.left, client.top, client.left, client.bottom);
        }
        //draw inner border
        client.left++;
        client.right--;
        client.top++;
        client.bottom--;
        switch (border_style()){
          case border_styles::raised:
          case border_styles::lowered:
            return;
          case border_styles::etched:
          case border_styles::bumped:
            std::swap(highlight, shadow);
          case border_styles::double_raised:
          case border_styles::double_lowered:
            if (_draw_right) dc.line(shadow, client.right, client.top, client.right, 1 + client.bottom);
            if (_draw_bottom) dc.line(shadow, client.left, client.bottom, client.right, client.bottom);
            if (_draw_top) dc.line(highlight, client.left, client.top, client.right, client.top);
            if (_draw_left) dc.line(highlight, client.left, client.top, client.left, client.bottom);
        }
        return;
      }

      virtual LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS * pSizes) override{
        pSizes->rgrc[0].top += border_width();
        pSizes->rgrc[0].left += border_width();
        pSizes->rgrc[0].bottom -= border_width();
        pSizes->rgrc[0].right -= border_width();
        return WVR_VALIDRECTS | WVR_REDRAW; 
      }

      virtual LRESULT on_wm_nccalcsize(RECT * pClient) override{
        return 0;
      }

    private:
      border_styles _border_style = border_styles::raised;
      rgb _border_highlight = system_rgb<system_colors::button_highlight>();
      rgb _border_shadow = system_rgb<system_colors::button_shadow>();
      bool _draw_top = true;
      bool _draw_left = true;
      bool _draw_right = true;
      bool _draw_bottom = true;
    };

  }

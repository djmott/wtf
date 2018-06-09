/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{

    /**
     * @brief Creates a border
     */
    template <typename _super_t>
    struct has_border :  _super_t{

    /**
     * @brief Gets the border width
     * @return int width of the border in pixels
     */
    virtual int border_width() const noexcept {
      switch (_border_style){
        case border_styles::none: return 0;
        case border_styles::flat:
        case border_styles::raised:
        case border_styles::lowered: return 1;
        default: return 2;
      }
    }
    /**
     * @brief Border highlight color
     * 
     * @return const rgb& highlight color
     */
    virtual const rgb& border_highlight() const noexcept { return _border_highlight; }
    /**
     * @brief Border highlight color
     * 
     * @param newval new highlight color
     */
    virtual void border_highlight(const rgb& newval) {
      _border_highlight = newval;
      refresh_border();
    }

    /**
     * @brief Border shadow color
     * 
     * @return const rgb& shadow color
     */
    virtual const rgb& border_shadow() const noexcept { return _border_shadow; }
    /**
     * @brief Border shadow color
     * 
     * @param newval new shadow color
     */
    virtual void border_shadow(const rgb& newval) {
      _border_shadow = newval;
      refresh_border();
    }

    /**
     * @brief Border style
     * 
     * @return border_styles border style
     */
    virtual border_styles border_style() const noexcept { return _border_style; }
    /**
     * @brief Border style
     * 
     * @param newval new border style
     */
    virtual void border_style(border_styles newval) {
      _border_style = newval;
      refresh_border();
    }

    /**
     * @brief Enables/disables drawing border edges
     * 
     * @param top enable or disable top edge
     * @param right enable or disable right edge
     * @param bottom enable or disable bottom edge
     * @param left enable or disable leftedge
     */
    virtual void enable_border_edges(bool top, bool right, bool bottom, bool left) noexcept {
      _draw_top = top; _draw_left = left; _draw_right = right; _draw_bottom = bottom;
    }

    protected:

      /**
       * @brief Construct a new has border object
       * 
       * @param pParent parent window
       */
      explicit has_border(window * pParent) noexcept : _super_t(pParent){}

      /**
       * @brief Invalidates the non-client area forcing a border redraw
       */
      void refresh_border()  {
        if (!_super_t::_handle) return;
        wtf::exception::throw_lasterr_if(
          ::RedrawWindow(*this, nullptr, nullptr, RDW_FRAME | RDW_INVALIDATE | RDW_NOCHILDREN),
          [](BOOL b)noexcept { return !b; }
        );
      }

      /**
       * @brief handles WM_NCPAINT messages
       * 
       * @param dc device contact of the non-client area
       * @param oClient drawing rect
       */
      void on_wm_ncpaint(const device_context& dc, const rect<coord_frame::client>& oClient) override {

        auto highlight = pen::create(pen::style::solid, 1, border_highlight());
        auto shadow = pen::create(pen::style::solid, 1, border_shadow());
        rect<coord_frame::client> client = oClient;
        client.right--;
        client.bottom--;
        wtf::_::effects::draw_border(dc, client, border_style(), highlight, shadow, _draw_left, _draw_top, _draw_right, _draw_bottom);

        return _super_t::on_wm_ncpaint(dc, oClient);
      }

      /**
       * @brief handles WM_NCCALCSIZE messages
       * 
       * @param pSizes contains information an application can use to calculate the new size and position of the client rectangle
       * @return LRESULT (WVR_VALIDRECTS | WVR_REDRAW) specifies valid rects are in pSizes->rgrc[1] and [2] plus forces redraw of the entire window
       */
      LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS * pSizes) override {
        pSizes->rgrc[0].top += border_width();
        pSizes->rgrc[0].left += border_width();
        pSizes->rgrc[0].bottom -= border_width();
        pSizes->rgrc[0].right -= border_width();
        return WVR_VALIDRECTS | WVR_REDRAW;
      }

      /**
       * @brief handles WM_NCCALCSIZE messages. Overloaded implementation not required.
       * 
       * @return LRESULT - 0 
       */
      LRESULT on_wm_nccalcsize(RECT *) override { return 0; }

    private:
      border_styles _border_style = border_styles::none;
      rgb _border_highlight = system_rgb<system_colors::button_highlight>();
      rgb _border_shadow = system_rgb<system_colors::button_shadow>();
      bool _draw_top = true;
      bool _draw_left = true;
      bool _draw_right = true;
      bool _draw_bottom = true;
    };

  }

  namespace _{
    template <> struct policy_traits<policy::has_border>{
      using requires = policy_list<policy::wm_ncpaint, policy::wm_nccalcsize>;
    };
  }
}
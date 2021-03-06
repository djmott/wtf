﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
#if !DOXY_INVOKED

namespace wtf{

  enum class custom_border_styles{
    none = 0,
    flat,
    raised,
    lowered,
    bumped,
    etched,
    double_raised,
    double_lowered,
  };

  enum class quadrants{
    left,
    top,
    right,
    bottom,
  };

  namespace _{

    struct effects{

      static void draw_arrow(const device_context& dc, const rect<coord_frame::client>& oclient, quadrants direction, pen& outline, brush& fill) {
        point<coord_frame::client>::vector arrow(3);

        switch (direction){
          case quadrants::left:
            arrow[0].x = oclient.left; arrow[0].y = oclient.top + ((oclient.bottom - oclient.top) / 2); 
            arrow[1].x = oclient.right; arrow[1].y = oclient.top;  
            arrow[2].x = oclient.right; arrow[2].y = oclient.bottom; 
            break;
          case quadrants::top:
            arrow[0].x = oclient.left; arrow[0].y = oclient.bottom;
            arrow[1].x = oclient.left + ((oclient.right - oclient.left) / 2); arrow[1].y = oclient.top;
            arrow[2].x = oclient.right; arrow[2].y = oclient.bottom;
            break;
          case quadrants::right:
            arrow[0].x = oclient.left; arrow[0].y = oclient.bottom;
            arrow[1].x = oclient.left; arrow[1].y = oclient.top;
            arrow[2].x = oclient.right; arrow[2].y = oclient.top + ((oclient.bottom - oclient.top) / 2);
            break;
          case quadrants::bottom:
            arrow[0].x = oclient.left; arrow[0].y = oclient.top;
            arrow[1].x = oclient.right; arrow[1].y = oclient.top;
            arrow[2].x = oclient.left + ((oclient.right - oclient.left) / 2); arrow[2].y = oclient.bottom;
            break;
        }
        dc.fill(arrow, outline, fill);

      }

      static void draw_border(const device_context& dc, const rect<coord_frame::client>& oclient, custom_border_styles style, const pen& highlight, const pen& shadow, bool left=true, bool top=true, bool right=true, bool bottom=true){
        bool bInvert = false;
        rect<coord_frame::client> client = oclient;
        //outer border
        switch (style){
          case custom_border_styles::none:
            return;
          case custom_border_styles::flat:
            if (right) dc.line(shadow, client.right, client.top, client.right, 1+client.bottom);
            if (bottom) dc.line(shadow, client.left, client.bottom, client.right, client.bottom);
            if (top) dc.line(shadow, client.left, client.top, client.right, client.top);
            if (left) dc.line(shadow, client.left, client.top, client.left, client.bottom);
            return;
          case custom_border_styles::etched:
          case custom_border_styles::lowered:
          case custom_border_styles::double_lowered:
            bInvert=true; //fall through
          case custom_border_styles::bumped:
          case custom_border_styles::raised:
          case custom_border_styles::double_raised:
            if (right) dc.line(bInvert ? highlight : shadow, client.right, client.top, client.right, 1 + client.bottom);
            if (bottom) dc.line(bInvert ? highlight : shadow, client.left, client.bottom, client.right, client.bottom);
            if (top) dc.line(bInvert ? shadow : highlight, client.left, client.top, client.right, client.top);
            if (left) dc.line(bInvert ? shadow : highlight, client.left, client.top, client.left, client.bottom);
        }
        //inner border
        client.left++;
        client.right--;
        client.top++;
        client.bottom--;
        switch (style){
          case custom_border_styles::flat:
          case custom_border_styles::none:
          case custom_border_styles::raised:
          case custom_border_styles::lowered:
            return;
          case custom_border_styles::etched:
          case custom_border_styles::bumped:
            bInvert = !bInvert;
          case custom_border_styles::double_raised:
          case custom_border_styles::double_lowered:
            if (right) dc.line(bInvert ? highlight : shadow, client.right, client.top, client.right, 1+client.bottom);
            if (bottom) dc.line(bInvert ? highlight : shadow, client.left, client.bottom, client.right, client.bottom);
            if (top) dc.line(bInvert ? shadow : highlight, client.left, client.top, client.right, client.top);
            if (left) dc.line(bInvert ? shadow : highlight, client.left, client.top, client.left, client.bottom);
        }
      }

    };

  }

}
#endif
/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_LABEL_SUPER \
  DOXY_INHERIT_WINDOW \
  DOXY_INHERIT_HAS_TEXT \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_ENABLE \
  DOXY_INHERIT_HAS_MOVE \
  DOXY_INHERIT_HAS_SIZE \
  DOXY_INHERIT_HAS_STYLE \
  DOXY_INHERIT_WM_SIZE \
  DOXY_INHERIT_WM_COMMAND


namespace wtf {
  namespace controls {

    /** @class label
    @brief A static control with text.
    @ingroup Widgets
    @image html label.png
    */
    struct label : DOXY_INHERIT_LABEL_SUPER window_impl<label,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_size,
      policy::has_style,
      policy::wm_size,
      policy::wm_command
    > {

      //! @brief gets the text horizontal alignment 
      text_horizontal_alignments alignment() const { 
        if (get_style<SS_LEFT>()) return text_horizontal_alignments::left;
        if (get_style<SS_CENTER>()) return text_horizontal_alignments::center;
        return text_horizontal_alignments::right;
      }

      //! @brief sets the text horizontal alignment
      void alignment(text_horizontal_alignments newval) {
        if (text_horizontal_alignments::left == newval) {
          set_style<SS_LEFT>(true);
        }
        else if (text_horizontal_alignments::center == newval) {
          set_style<SS_CENTER>(true);
        }
        else {
          set_style<SS_RIGHT>(true);
        }
      }

      //! @brief draws an image
      void set_image(const icon& newval) {
        set_style<SS_BITMAP | SS_ENHMETAFILE>(false);
        set_style<SS_ICON>(true);
        ::SendMessage(*this, STM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(static_cast<HICON>(newval)));
      }
      //! @brief draws an image
      //! @overload
      void set_image(const bitmap& newval) {
        ::SendMessage(*this, STM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(static_cast<HBITMAP>(newval)));
      }

    protected:
      void on_created() override {
        alignment(text_horizontal_alignments::left);
        __super::on_created();
      }
    };
    
  }


#if !DOXY_INVOKED

  namespace _ {
#if WTF_USE_COMMON_CONTROLS
    TCHAR sWC_STATIC[] = WC_STATIC;
#else
    TCHAR sWC_STATIC[] = _T("STATIC");
#endif
  }

  template <WNDPROC window_proc> struct window_class<controls::label, window_proc> 
    : super_window_class<_::sWC_STATIC, controls::label, window_proc> {};

#endif
}

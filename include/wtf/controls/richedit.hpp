/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {

      TCHAR sMSFTEDIT_CLASS[] = MSFTEDIT_CLASS;

      template <typename _impl_t> using richedit_impl = window_impl<_impl_t,
        policy::has_border,
        policy::has_font,
        policy::has_text,
        policy::has_enable,
        policy::has_style,
        policy::has_exstyle,
        wtf::policy::has_move,
        wtf::policy::nm_killfocus,
        wtf::policy::nm_setfocus,
        wtf::policy::wm_notify
      >;

    }

    /** @class richedit
    @ingroup Widgets
    @brief A rich edit control enables the user to enter, edit, print, and save text.
    @tparam _multiline indicates that multiple lines of text are permitted.
    */
    template <bool _multiline>
    struct richedit : _::richedit_impl<richedit<_multiline>> {

      using char_range = std::tuple<uint32_t, uint32_t>;

      static constexpr DWORD Style = window::Style | (_multiline ? ES_MULTILINE : 0);

      richedit() : _::richedit_impl<richedit<_multiline>>() {
        wtf::exception::throw_lasterr_if(::LoadLibrary(_T("msftedit.dll")), [](HMODULE h) { return nullptr == h; });
      }

      struct paragraph_formatting : PARAFORMAT2{
        paragraph_formatting() {
          memset(this, 0, sizeof(PARAFORMAT2));
          cbSize = sizeof(PARAFORMAT2);
          dwMask = PFM_ALL2;
        }
      };

      struct character_formatting : CHARFORMAT2 {
        character_formatting() {
          memset(this, 0, sizeof(CHARFORMAT2));
          cbSize = sizeof(CHARFORMAT2);
          dwMask = CFM_ALL2;
        }
      };

      enum class event_mask_flags {
        change = ENM_CHANGE,
        clip_format = ENM_CLIPFORMAT,
        correct_text = ENM_CORRECTTEXT,
        drag_drop_done = ENM_DRAGDROPDONE,
        drop_files = ENM_DROPFILES,
        ime_change = ENM_IMECHANGE,
        key_events = ENM_KEYEVENTS,
        link = ENM_LINK,
        lowfirtf = ENM_LOWFIRTF,
        mouse_events = ENM_MOUSEEVENTS,
        object_positions = ENM_OBJECTPOSITIONS,
        paragraphe_expanded = ENM_PARAGRAPHEXPANDED,
        text_protected = ENM_PROTECTED,
        request_size = ENM_REQUESTRESIZE,
        scroll = ENM_SCROLL,
        scroll_events = ENM_SCROLLEVENTS,
        sel_change = ENM_SELCHANGE,
        update = ENM_UPDATE,
      };

      event_mask_flags event_mask() const { return static_cast<event_mask_flags>(::SendMessage(*this, EM_GETEVENTMASK, 0, 0)); }

      void event_mask(event_mask_flags newval) { ::SendMessage(*this, EM_SETEVENTMASK, 0, static_cast<LPARAM>(newval)); }

      bool can_paste() const { return ::SendMessage(*this, EM_CANPASTE, 0, 0) ? true : false; }

      bool can_redo() const { return ::SendMessage(*this, EM_CANREDO, 0, 0) ? true : false; }

      char_range selection() const {
        CHARRANGE cr;
        ::SendMessage(*this, EM_EXGETSEL, 0, reinterpret_cast<LPARAM>(&cr));
        return std::make_tuple<uint32_t, uint32_t>(static_cast<uint32_t>(cr.cpMin), static_cast<uint32_t>(cr.cpMax));
      }

      void selection(char_range newval) {
        CHARRANGE cr;
        cr.cpMin = std::get<0>(newval);
        cr.cpMax = std::get<1>(newval);
        ::SendMessage(*this, EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&cr));
      }

      void selection(uint32_t start, uint32_t stop) { selection(std::make_tuple(start, stop)); }

      void max_chars(uint16_t newval) { ::SendMessage(*this, EM_EXLIMITTEXT, 0, newval); }

      //! Disables scroll bars instead of hiding them when they are not needed.
      bool disable_noscroll() const { return _::richedit_impl<richedit<_multiline>>::template get_style<ES_DISABLENOSCROLL>(); }

      //! Disables scroll bars instead of hiding them when they are not needed.
      void disable_noscroll(bool newval) { _::richedit_impl<richedit<_multiline>>::template set_style<ES_DISABLENOSCROLL>(newval); }

      //! Preserves the selection when the control loses the focus. By default, the entire contents of the control are selected when it regains the focus.
      bool save_selection() const { return _::richedit_impl<richedit<_multiline>>::template get_style<ES_SAVESEL>(); }

      //! Preserves the selection when the control loses the focus. By default, the entire contents of the control are selected when it regains the focus.
      void save_selection(bool newval) { return _::richedit_impl<richedit<_multiline>>::template set_style<ES_SAVESEL>(newval); }

      bool auto_hscroll() const { return _::richedit_impl<richedit<_multiline>>::get_style<ES_AUTOHSCROLL>(); }
      void auto_hscroll(bool newval) { return _::richedit_impl<richedit<_multiline>>::template set_style<ES_AUTOHSCROLL>(newval); }

      void show_hscroll(bool newval) const { ::SendMessage(*this, EM_SHOWSCROLLBAR, SB_HORZ, (newval ? TRUE : FALSE)); }

      bool auto_vscroll() const { return _::richedit_impl<richedit<_multiline>>::get_style<ES_AUTOVSCROLL>(); }
      void auto_vscroll(bool newval) { return _::richedit_impl<richedit<_multiline>>::template set_style<ES_AUTOVSCROLL>(newval); }

      void show_vscroll(bool newval) const { ::SendMessage(*this, EM_SHOWSCROLLBAR, SB_VERT, (newval ? TRUE : FALSE)); }


      bool want_return() const { return _::richedit_impl<richedit<_multiline>>::get_style<ES_WANTRETURN>(); }
      void want_return(bool newval) { return _::richedit_impl<richedit<_multiline>>::template set_style<ES_WANTRETURN>(newval); }

      uint32_t text_length() const {
        GETTEXTLENGTHEX gt;
        gt.flags = GTL_NUMCHARS;
#if defined(_UNICODE)
        gt.codepage = CP_WINUNICODE;
#else
        gt.codepage = CP_ACP;
#endif
        return static_cast<uint32_t>(::SendMessage(*this, EM_GETTEXTLENGTHEX, reinterpret_cast<WPARAM>(&gt), 0));
      }

      tstring text() const {
        GETTEXTEX gt;
        memset(&gt, 0, sizeof(GETTEXTEX));
        tstring sRet(text_length(), 0);

        gt.cb = static_cast<DWORD>((sRet.size() + 1) * sizeof(TCHAR));
        gt.flags = GT_USECRLF;
#if defined(_UNICODE)
        gt.codepage = CP_WINUNICODE;
#else
        gt.codepage = CP_ACP;
        gt.lpDefaultChar = "?";
#endif
        ::SendMessage(*this, EM_GETTEXTEX, reinterpret_cast<WPARAM>(&gt), reinterpret_cast<LPARAM>(&sRet[0]));
        return sRet;
      }

      void text(const tstring& newval) {
        SETTEXTEX st;        
        st.flags = ST_KEEPUNDO;
#if defined(_UNICODE)
        st.flags |= ST_UNICODE;
        st.codepage = CP_WINUNICODE;
#else
        st.codepage = CP_ACP;
#endif
        wtf::exception::throw_lasterr_if(::SendMessage(*this, EM_SETTEXTEX, reinterpret_cast<WPARAM>(&st), reinterpret_cast<LPARAM>(newval.c_str())), [](LRESULT l) { return !l; });
      }

      paragraph_formatting paragraph_format() const {
        paragraph_formatting oRet;
        ::SendMessage(*this, EM_GETPARAFORMAT, 0, &oRet);
        return oRet;
      }

      void paragraph_format(const paragraph_formatting& newval) {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, EM_SETPARAFORMAT, 0, reinterpret_cast<LPARAM>(&newval)), [](LRESULT l) { return !l; });
      }

      character_formatting character_format() const {
        character_formatting oRet;
        ::SendMessage(*this, EM_GETCHARFORMAT, SCF_SELECTION, &oRet);
      }

      void character_format(const character_formatting& newval) {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&newval)), [](LRESULT l) { return !l; });
      }
    protected:
      void handle_msg(wtf::window_message& msg) override {
        if (WM_COMMAND != msg.umsg) return;
        //this control must not forward parent redirected WM_COMMAND messages
        msg.bhandled = (msg.hwnd != window::_handle);
        if (!msg.bhandled) DebugBreak();
      }

      void on_wm_notify(NMHDR * notification) override {
        if (EN_SELCHANGE == notification->code) return;
      }

    };

  }

  template <bool _multiline, WNDPROC window_proc>
  struct window_class<controls::richedit<_multiline>, window_proc> :
    super_window_class<controls::_::sMSFTEDIT_CLASS, controls::richedit<_multiline>, window_proc> {};

}
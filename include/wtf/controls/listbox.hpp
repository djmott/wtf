/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_LISTBOX_SUPER \
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

    /** @class listbox
    @brief Displays a selectable list of items
    @ingroup Widgets
    
    */
    struct listbox : DOXY_INHERIT_LISTBOX_SUPER window_impl<listbox,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_size,
      policy::has_style,
      policy::wm_size,
      policy::wm_command
    > {


      struct item {
        using pointer = std::shared_ptr<item>;

        const tstring& text() const { return _text; }
        void text(const tstring& newval) { _text = newval; }


        struct collection {
          size_t size() const { return _inner.size(); }
        private:
          std::vector<typename item::pointer> _inner;
        };

      private:
        friend struct listbox;
        tstring _text;
      };



    protected:
      item::collection _items;
    };

  }


#if !DOXY_INVOKED

  namespace _ {
#if WTF_USE_COMMON_CONTROLS
    TCHAR sWC_LISTBOX[] = WC_LISTBOX;
#else
    TCHAR sWC_LISTBOX[] = _T("LISTBOX ");
#endif
  }

  template <WNDPROC window_proc> struct window_class<controls::listbox, window_proc>
    : super_window_class<_::sWC_LISTBOX, controls::listbox, window_proc> {};

#endif
}

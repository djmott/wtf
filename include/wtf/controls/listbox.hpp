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
    @ingroup Controls
    
    */
    struct listbox : DOXY_INHERIT_LISTBOX_SUPER window_impl<listbox,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_size,
      policy::has_style,
      messages::wm_size,
      messages::wm_command
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
      template <typename, template <typename> typename...> friend struct window_impl;
#if WTF_USE_COMMON_CONTROLS
      static constexpr TCHAR sub_window_class_name[] = WC_LISTBOX;
#else
      static constexpr TCHAR sub_window_class_name[] = LISTBOX;
#endif
      static constexpr TCHAR window_class_name[] = _T("wtf_listbox");
      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;
      item::collection _items;
    };

  }

}

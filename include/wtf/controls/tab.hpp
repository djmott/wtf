/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_TAB_SUPER \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_MOVE \
  DOXY_INHERIT_HAS_STYLE \
  DOXY_INHERIT_WM_NOTIFY \
  DOXY_INHERIT_WM_SIZE

namespace wtf {
  namespace controls {

    /** @class tab
    @brief A tab control is analogous to the dividers in a notebook or the labels in a file cabinet. By using a tab control, an application can define multiple pages for the same area of a window or dialog box.
    @ingroup Controls
    */

    struct tab : DOXY_INHERIT_TAB_SUPER window_impl<tab,
       policy::has_font,
      policy::has_move,
      policy::has_style,
      messages::wm_notify,
      messages::wm_size
    > {
      static const DWORD ExStyle = window::ExStyle | WS_EX_CONTROLPARENT;

      static constexpr TCHAR sub_window_class_name[] = WC_TABCONTROL;
      static constexpr TCHAR window_class_name[] = _T("wtf_tab");
      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;

      /** @class item
      @brief represents a single page in the tab control
      */
      struct item {

        const tstring& text() const noexcept { return _text; }
        
        struct collection {
          size_t count() const { return _inner.size(); }
          item& operator[](size_t index) { return *_inner[index]; }
          const item& operator[](size_t index) const { return *_inner[index]; }
          template <typename _ty>
          void add(const tstring& text, _ty& newval) {
            _inner.push_back(std::unique_ptr<item>(new item(text, &newval)));
            TCITEM oTCItem;
            oTCItem.pszText = const_cast<LPTSTR>(text.c_str());
            oTCItem.mask = TCIF_TEXT;
            wtf::exception::throw_lasterr_if(::SendMessage(*_parent, TCM_INSERTITEM, _inner.size(), reinterpret_cast<LPARAM>(&oTCItem)), [](LRESULT l) { return -1 == l; });
            _parent->resize_children();
          }
          void add(const tstring& text) {
            _inner.push_back(std::unique_ptr<item>(new item(text, nullptr)));
            TCITEM oTCItem;
            oTCItem.pszText = const_cast<LPTSTR>(text.c_str());
            oTCItem.mask = TCIF_TEXT;
            wtf::exception::throw_lasterr_if(::SendMessage(*_parent, TCM_INSERTITEM, _inner.size(), reinterpret_cast<LPARAM>(&oTCItem)), [](LRESULT l) { return -1 == l; });
            _parent->resize_children();
          }
        protected:
          friend struct tab;
          friend struct item;
          collection(tab * parent) : _parent(parent){}
          std::vector<std::unique_ptr<item>> _inner;
          tab * _parent;
        };

      protected:
        template <typename, template <typename> typename...> friend struct window_impl;
        friend struct tab;
        item(const tstring& stext, window * pwindow) : _text(stext), _window(pwindow){}
        tstring _text;
        window * _window;
      };

      tab() : _items(this) {}

      //! @brief invoked when the user clicks the control
      callback<void(window*)> OnClick;
      //! @brief invoked when the user double-clicks the control
      callback<void(window*)> OnDblClick;
      //! @brief invoked when the active tab is about to change
      callback<void(window*)> OnChanging;
      //! @brief invoked when the active tab has changed
      callback<void(window*)> OnChanged;

      //! @brief determines if the control can receive input focus when clicked
      bool recieves_focus() const { return !get_style_bit<TCS_FOCUSNEVER>(); }
      //! @brief enables or disables input focus behavior
      void recieves_focus(bool newval) { set_style_bit<TCS_FOCUSNEVER>(!newval); }

      //! @brief gets the flat button style
      bool flat_buttons() const { return get_style_bit<TCS_FLATBUTTONS>(); }
      //! @brief sets the flat button style
      void flat_buttons(bool newval) { return set_style_bit<TCS_FLATBUTTONS>(newval); }

      //! @brief gets the multi-line style
      bool multi_line() const { return get_style_bit<TCS_MULTILINE>(); }
      //! @brief sets the multi-line style
      void multi_line(bool newval) { return set_style_bit<TCS_MULTILINE>(newval); }

      //! @brief gets the placement of the tab buttons
      wtf::quadrants placement() const {
        auto iStyle = get_style_value() & (TCS_BOTTOM | TCS_VERTICAL | TCS_MULTILINE);
        switch (iStyle) {
          case 0: return quadrants::top;
          case TCS_BOTTOM: return quadrants::bottom;
          case (TCS_VERTICAL | TCS_MULTILINE): return quadrants::left;
          default: return quadrants::right;
        }
      }
      //! @brief sets the placement of the tab buttons
      void placement(wtf::quadrants newval) {
        auto iStyle = get_style_value() & ~(TCS_BOTTOM | TCS_VERTICAL | TCS_MULTILINE);
        switch (newval) {
          case quadrants::top: break;
          case quadrants::bottom: iStyle |= TCS_BOTTOM; break;
          case quadrants::left: iStyle |= TCS_VERTICAL; break;
          default: iStyle |= (TCS_VERTICAL | TCS_RIGHT); break;
        }
        set_style_value(iStyle);
      }

      //! @brief gets the index of the active tab page
      size_t current_index() const {
        return static_cast<size_t>(::SendMessage(*this, TCM_GETCURSEL, 0, 0));
      }

      //! @brief gets the active tab page
      const item& current_item() const { return _items[current_index()]; }

      //! @brief gets the collection of tab pages
      item::collection& items() { return _items; }
      //! @brief gets the collection of tab pages
      const item::collection& items() const { return _items; }

    protected:

      item::collection _items;

      void on_wm_size(wm_size_flags flags, const point<coord_frame::client>& p) override {
         resize_children();
        __super::on_wm_size(flags, p);
      }

      void resize_children() {
        if (!_items._inner.size()) return;
        const auto x = current_index();
        if (-1 == x) return;
        auto r = wtf::rect<coord_frame::client>::get(*this);
        ::SendMessage(*this, TCM_ADJUSTRECT, FALSE, reinterpret_cast<LPARAM>(&r));

        for (size_t i = 0; i < _items._inner.size(); ++i) {
          auto & oItem = _items._inner[i];
          if (!oItem->_window) continue;
          if (i == x) {
            ::ShowWindow(*oItem->_window, SW_SHOW);
            wtf::exception::throw_lasterr_if(::SetWindowPos(*oItem->_window, HWND_TOP, r.left, r.top, r.right - r.left, r.bottom - r.top, SWP_SHOWWINDOW), [](BOOL b) { return !b; });
          } else {
            ::ShowWindow(*oItem->_window, SW_HIDE);
          }
        }
      }

      void handle_msg(wtf::window_message& msg) override {
        if (WM_NOTIFY != msg.umsg) return;
        auto notification = reinterpret_cast<NMHDR*>(msg.lparam);
        switch (notification->code) {
          case NM_CLICK: OnClick(this); resize_children(); break;
          case NM_DBLCLK: OnDblClick(this); resize_children(); break;
          case TCN_SELCHANGE: OnChanged(this); resize_children();  break;
          case TCN_SELCHANGING: OnChanging(this); resize_children();  break;
        }
        if (msg.hwnd != window::_handle) {
          msg.bhandled = true;
        }
      }
    };

  }

}
/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {
      
      TCHAR sWC_TABCONTROL[] = WC_TABCONTROL;

    }

    /** @class tab
    @ingroup Widgets
    @brief A tab control is analogous to the dividers in a notebook or the labels in a file cabinet. By using a tab control, an application can define multiple pages for the same area of a window or dialog box.
    */
    struct tab : window_impl<tab,
      policy::has_font,
      policy::has_move,
      policy::wm_notify,
      policy::wm_size
    > {
      static constexpr DWORD ExStyle = WS_EX_CONTROLPARENT;
      static constexpr DWORD Style = window::Style | TCS_FOCUSNEVER;


      tab() : _items(this){}

      callback<void(window*)> OnClick;
      callback<void(window*)> OnDblClick;
      callback<void(window*)> OnChanging;
      callback<void(window*)> OnChanged;

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


      size_t current_index() const {
        return static_cast<size_t>(::SendMessage(*this, TCM_GETCURSEL, 0, 0));
      }

      const item& current_item() const { return _items[current_index()]; }

      item::collection& items() { return _items; }
      const item::collection& items() const { return _items; }

    protected:

      item::collection _items;

      void on_wm_size(wm_size_flags flags, const point<coord_frame::client>& p) override {
        resize_children();
        __super::on_wm_size(flags, p);
      }

      void resize_children() {
        if (!_items._inner.size()) return;
        const auto i = current_index();
        if (-1 == i) return;

        for (const auto & Item : _items._inner) {
          if (Item->_window) wtf::exception::throw_lasterr_if(::SetWindowPos(*Item->_window, HWND_BOTTOM, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE), [](BOOL b) { return !b; });
        }

        auto r = wtf::rect<coord_frame::client>::get(*this);
        ::SendMessage(*this, TCM_ADJUSTRECT, FALSE, reinterpret_cast<LPARAM>(&r));
        const auto & Item = _items[i];
        if (Item._window) {
          wtf::exception::throw_lasterr_if(::SetWindowPos(*Item._window, HWND_TOP, r.left, r.top, r.right - r.left, r.bottom - r.top, SWP_SHOWWINDOW), [](BOOL b) { return !b; });
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

  template <WNDPROC window_proc> struct window_class<controls::tab, window_proc> : super_window_class<controls::_::sWC_TABCONTROL, controls::tab, window_proc> {};

}
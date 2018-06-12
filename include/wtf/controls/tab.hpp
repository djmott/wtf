/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {
      
      TCHAR sWC_TABCONTROL[] = _T(WC_TABCONTROL);

      template <typename _impl_t> using tab_impl = window_impl<_impl_t,
        policy::has_font,
        wtf::policy::has_move,
        wtf::policy::wm_notify,
        wtf::policy::wm_size
      >;

    }

    struct tab : _::tab_impl<tab> {
      static constexpr DWORD ExStyle = WS_EX_CONTROLPARENT;
      static constexpr DWORD Style = TCS_FOCUSNEVER | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;

      tab(window * parent) : _::tab_impl<tab>(parent) {
      }

      callback<void(window*)> OnClick;
      callback<void(window*)> OnDblClick;
      callback<void(window*)> OnChanging;
      callback<void(window*)> OnChanged;

      struct item {
        using pointer = std::shared_ptr<item>;
        using vector = std::vector<pointer>;
        virtual ~item() = default;
        item() = default;
        item(const item&) = default;
        item(item&&) = default;
        item& operator=(const item&) = default;
        item& operator=(item&&) = default;
        item(const tstring& Text, window * ChildWindow) : _text(Text), _window(ChildWindow){}

        const tstring& text() const noexcept { return _text; }
        wtf::window * window() const noexcept { return _window; }
      protected:
        template <typename, template <typename> typename...> friend struct window_impl;

        tstring _text;
        wtf::window * _window;
      };


      item * get_item(int index) const { return _items[index].get(); }

      int current_index() const {
        return static_cast<int>(::SendMessage(*this, TCM_GETCURSEL, 0, 0));
      }

      item * current_item() const { return get_item(current_index()); }

      item::pointer add_item(typename item::pointer Item) {
        TCITEM oTCItem;
        assert(Item->window());
        oTCItem.pszText = const_cast<LPTSTR>(Item->text().c_str());
        oTCItem.mask = TCIF_TEXT;
        wtf::exception::throw_lasterr_if(::SendMessage(*this, TCM_INSERTITEM, _items.size(), reinterpret_cast<LPARAM>(&oTCItem)), [](LRESULT l) { return -1 == l; });
        _items.push_back(Item);
        window::add(Item->window());
        resize_children();
        return Item;
      }

      item::pointer add_item(const tstring& text, window * child) {
        return add_item(std::make_shared<item>(text, child));
      }

    protected:

      typename item::vector _items;

      void on_wm_size(const point<coord_frame::client>& p) override {
        resize_children();
        _::tab_impl<tab>::on_wm_size(p);
      }

      void resize_children() {
        if (!_items.size()) return;
        const auto i = current_index();
        if (-1 == i) return;
        for (const auto & Item : _items) {
          ::ShowWindow(*Item->window(), SW_HIDE);
        }
        auto r = wtf::rect<coord_frame::client>::get(*this);
        ::SendMessage(*this, TCM_ADJUSTRECT, FALSE, reinterpret_cast<LPARAM>(&r));
        auto Item = get_item(i);
        wtf::exception::throw_lasterr_if(::MoveWindow(*Item->window(), r.left, r.top, r.right - r.left, r.bottom - r.top, FALSE), [](BOOL b) { return 0 == b; });
        ::ShowWindow(*Item->window(), SW_SHOW);
      }

      void on_wm_notify(NMHDR * notification) override {
        switch (notification->code) {
          case NM_CLICK: OnClick(this); resize_children(); break;
          case NM_DBLCLK: OnDblClick(this); resize_children(); break;
          case TCN_SELCHANGE: OnChanged(this); resize_children();  break;
          case TCN_SELCHANGING: OnChanging(this); resize_children();  break;
        }
        _::tab_impl<tab>::on_wm_notify(notification);
      }
    };

  }

  template <WNDPROC window_proc> struct window_class<controls::tab, window_proc> : super_window_class<controls::_::sWC_TABCONTROL, controls::tab, window_proc> {};

}
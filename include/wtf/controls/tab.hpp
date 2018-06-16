/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {
      
      TCHAR sWC_TABCONTROL[] = WC_TABCONTROL;

      template <typename _impl_t> using tab_impl = window_impl<_impl_t,
        policy::has_font,
        wtf::policy::has_move,
        wtf::policy::wm_notify,
        wtf::policy::wm_size
      >;

    }

    /** @class tab
    @ingroup Widgets
    @brief A tab control is analogous to the dividers in a notebook or the labels in a file cabinet. By using a tab control, an application can define multiple pages for the same area of a window or dialog box.
    */
    struct tab : _::tab_impl<tab> {
      static constexpr DWORD ExStyle = WS_EX_CONTROLPARENT;
      static constexpr DWORD Style = TCS_FOCUSNEVER | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;

      tab() : _::tab_impl<tab>(), _items(this){
        wtf::_::init_common_controls<wtf::_::tab_classes>::get();
      }

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

      /*
      item * get_item(int index) const { return _items[index].get(); }


      item::pointer add_item(typename item::pointer Item) {
        TCITEM oTCItem;
        oTCItem.pszText = const_cast<LPTSTR>(Item->text().c_str());
        oTCItem.mask = TCIF_TEXT;
        wtf::exception::throw_lasterr_if(::SendMessage(*this, TCM_INSERTITEM, _items.size(), reinterpret_cast<LPARAM>(&oTCItem)), [](LRESULT l) { return -1 == l; });
        _items.push_back(Item);
        resize_children();
        return Item;
      }

      item::pointer add_item(const tstring& text, ) {
        return add_item(std::make_shared<item>(text));
      }
      */

      item::collection& items() { return _items; }
      const item::collection& items() const { return _items; }

    protected:

      item::collection _items;

      void on_wm_size(const point<coord_frame::client>& p) override {
        resize_children();
        _::tab_impl<tab>::on_wm_size(p);
      }

      void resize_children() {
        if (!_items._inner.size()) return;
        const auto i = current_index();
        if (-1 == i) return;

        for (const auto & Item : _items._inner) {
          ::ShowWindow(*Item->_window, SW_HIDE);
        }

        auto r = wtf::rect<coord_frame::client>::get(*this);
        ::SendMessage(*this, TCM_ADJUSTRECT, FALSE, reinterpret_cast<LPARAM>(&r));
        const auto & Item = _items[i];
         wtf::exception::throw_lasterr_if(::MoveWindow(*Item._window, r.left, r.top, r.right - r.left, r.bottom - r.top, FALSE), [](BOOL b) { return 0 == b; });
         ::ShowWindow(*Item._window, SW_SHOW);
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
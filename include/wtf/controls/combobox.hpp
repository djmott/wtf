/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {

      template <typename _super_t> struct isa_combobox : _super_t {
        explicit isa_combobox(window * pParent) : window_impl(pParent) {}
        template <typename ... _arg_ts> isa_combobox(_arg_ts&&...args) noexcept : _super_t(std::forward<_arg_ts>(args)...) {}

        struct item {
          using vector = std::vector<item>;
          virtual ~item() = default;
          item() = default;
          item(const item&) = default;
          item(item&&) = defaultl;
          item& operator=(const item&) = default;
          item& operator=(item&&) = default;
          const tstring& text() const noexcept { return _text; }
          void text(const tstring& newval) { _text = newval; }
        private:
          tstring _text;
        };

        void clear() { ::SendMessage(*this, CB_RESETCONTENT, 0, 0); }

        const item * selected_item() const {
          iSelected = ::SendMessage(*this, CB_GETCURSEL, 0, 0);
          return (-1 == iSelected) ? nullptr : _items[iSelected];
        }

        void add(const item& newval) {
          _items.push_back(newval);
          if (!_sorted) {
            ::SendMessage(*this, CB_ADDSTRING, 0, _items.back().c_str());
            return;
          }
          size_t x
          vector::iterator it
          auto pSelected = selected_item();
          clear();
          std::sort(_items.begin(), _items.end());
          size_t x = -1;
          for (size_t i = 0; i < _items.size(); ++i) {
            if ( _items[i]  )
            ::SendMessage(*this, CB_ADDSTRING, 0, _items[i]->text().c_str());
          }
        }

        bool sorted() const { return _sorted; }
        void sorted(bool newval) { _sorted = newval; if (newval) sort(); }

        

      private:

        item::vector _items;
        bool _sorted = true;
      };


      template <typename _impl_t> using combobox_super_t = window_impl<_impl_t,
        policy::isa_combobox,
        policy::has_text
      >;

    }


    struct simple_combobox : policy::combobox_super_t<simple_combobox> {
      static constexpr DWORD Style = policy::combobox_super_t<simple_combobox>::Style | CBS_SIMPLE;
      simple_combobox(window * parent) : policy::combobox_super_t<simple_combobox>(parent) {}
    };

    struct edit_combobox : policy::combobox_super_t<edit_combobox> {
      static constexpr DWORD Style = policy::combobox_super_t<simple_combobox>::Style | CBS_DROPDOWN;
      edit_combobox(window * parent) : policy::combobox_super_t<edit_combobox>(parent) {}
    };

    struct static_combobox : policy::combobox_super_t<static_combobox> {
      static constexpr DWORD Style = policy::combobox_super_t<static_combobox>::Style | CBS_DROPDOWNLIST;
      static_combobox(window * parent) : policy::combobox_super_t<static_combobox>(parent) {}
    };


  }
#define _DEFINE_COMBO_WND_CLASS(_class) \
    template <WNDPROC window_proc> struct window_class_ex<_class, window_proc> { \
      constexpr LPCTSTR name() const noexcept { return WC_COMBOBOX; }  \
      static window_class_ex& get() {  \
        static window_class_ex _window_class_ex;  \
        return _window_class_ex;  \
      }};

  _DEFINE_COMBO_WND_CLASS(controls::simple_combobox);
  _DEFINE_COMBO_WND_CLASS(controls::edit_combobox);
  _DEFINE_COMBO_WND_CLASS(controls::static_combobox);
}
/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {

      template <typename _super_t> struct isa_combobox : _super_t {

        static constexpr DWORD ExStyle = 0;

        explicit isa_combobox(window * pParent) : _super_t(pParent) {
          wtf::_::init_common_controls<wtf::_::standard_classes>::get();
        }

        isa_combobox() : _super_t() {}

        struct item {
          using pointer = std::shared_ptr<item>;
          using vector = std::vector<pointer>;
          virtual ~item() = default;
          explicit item(const tstring& val) : _text(val) {}
          item() = default;
          item(const item&) = default;
          item(item&&) = default;
          item& operator=(const item&) = default;
          item& operator=(item&&) = default;
          const tstring& text() const noexcept { return _text; }
          void text(const tstring& newval) { _text = newval; }
          bool operator<(const item& rhs) const noexcept { return std::less<tstring>()(_text, rhs._text); }
        private:
          tstring _text;
        };

        void clear() noexcept { _items.clear(); ::SendMessage(*this, CB_RESETCONTENT, 0, 0); }

        const typename item::pointer selected_item() const noexcept {
          auto i = ::SendMessage(*this, CB_GETCURSEL, 0, 0);
          return (-1 == i) ? item::pointer() : _items[i];
        }

        void add_item(const item& newval) {
          _items.push_back(std::make_shared<item>(newval));
          wtf::exception::throw_lasterr_if(::SendMessage(*this, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_items.back()->text().c_str())), [](LRESULT l) { return CB_ERR == l || CB_ERRSPACE == l; });
        }

        void add_item(const tstring& newval) { add_item(item(newval)); }


        callback<void(window*)> OnCloseUp;
        callback<void(window*)> OnDblClick;
        callback<void(window*)> OnDropDown;
        callback<void(window*)> OnEditChange;
        callback<void(window*)> OnEditUpdate;
        callback<void(window*)> OnKillFocus;
        callback<void(window*)> OnSelChange;
        callback<void(window*)> OnSetFocus;

      private:
        void on_wm_command(WPARAM wparam, LPARAM lparam) override {
          switch (HIWORD(wparam)) {
          case CBN_CLOSEUP: OnCloseUp(this); break;
          case CBN_DBLCLK: OnDblClick(this); break;
          case CBN_DROPDOWN: OnDropDown(this); break;
          case CBN_EDITCHANGE: OnEditChange(this); break;
          case CBN_EDITUPDATE: OnEditUpdate(this); break;
          case CBN_KILLFOCUS: OnKillFocus(this); break;
          case CBN_SETFOCUS: OnSelChange(this); break;
          }
          _super_t::on_wm_command(wparam, lparam);
        }

        typename item::vector _items;
      };

      template <typename _impl_t> using combobox_super_t = window_impl<_impl_t,
        policy::isa_combobox,
        policy::has_font,
        wtf::policy::has_enable,
        wtf::policy::has_move,
        wtf::policy::wm_command
      >;
    }

    enum class combobox_styles {
      simple,
      drop_down,
      drop_down_list
    };

    template <bool _sorted, combobox_styles _style>
    struct combobox : policy::combobox_super_t<combobox<_sorted, _style>> {
      static constexpr bool sorted = _sorted;
      static constexpr DWORD Style = window::Style | WS_VSCROLL | CBS_AUTOHSCROLL |
        (_sorted ? CBS_SORT : 0) |
        (combobox_styles::simple == _style ? CBS_SIMPLE : 0) |
        (combobox_styles::drop_down == _style ? CBS_DROPDOWN : 0) |
        (combobox_styles::drop_down_list == _style ? CBS_DROPDOWNLIST : 0);
      combobox() : policy::combobox_super_t<combobox<_sorted, _style>>() {}
    };
  }


  namespace _ { TCHAR sWC_COMBOBOX[] = WC_COMBOBOX; }

  template <bool _sorted, controls::combobox_styles _style, WNDPROC window_proc>
  struct window_class<controls::combobox<_sorted, _style>, window_proc> : super_window_class<_::sWC_COMBOBOX, controls::combobox<_sorted, _style>, window_proc> {};

}
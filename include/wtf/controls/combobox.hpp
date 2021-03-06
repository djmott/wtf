﻿/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_COMBOBOX_SUPER \
  DOXY_INHERIT_WINDOW \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_ENABLE \
  DOXY_INHERIT_HAS_MOVE \
  DOXY_INHERIT_HAS_STYLE \
  DOXY_INHERIT_WM_COMMAND


namespace wtf {

  namespace controls {

    /** @brief Displays a list of items and one item can be actively selected
    @ingroup Controls
    @image html simple_combobox.png
    @image html dropdown_combobox.png
    @image html dropdown_list_combobox.png
    */
    struct combobox : DOXY_INHERIT_COMBOBOX_SUPER window_impl < combobox,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_style,
      messages::wm_command
    > {

      /** @brief represents a single item in a combobox that can be selected
      */
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

      combobox() {
        window::_style |= CBS_DROPDOWN;
      }

      enum class styles {
        simple = CBS_SIMPLE, //!< The list is always displayed. This style may not work properly in some cases.
        dropdown = CBS_DROPDOWN, //!< Default. The list is displayed when the drop-down button is pressed and the text box can be edited
        dropdown_list = CBS_DROPDOWNLIST,//!< The list is displayed when the drop-down button is pressed and the text box is read-only
      };

      //! @brief gets the style
      styles style() const {
        return static_cast<styles>(window::_style & (CBS_SIMPLE | CBS_DROPDOWN | CBS_DROPDOWNLIST));
      }

      //! @brief sets the style
      //! @details NOTE: this only takes effect when set prior to calling run()
      void style(styles newval) {
        window::_style &= ~(CBS_SIMPLE | CBS_DROPDOWN | CBS_DROPDOWNLIST);
        window::_style |= (styles::simple == newval ? CBS_SIMPLE : 0);
        window::_style |= (styles::dropdown == newval ? CBS_DROPDOWN : 0);
        window::_style |= (styles::dropdown_list == newval ? CBS_DROPDOWNLIST : 0);
      }

      //! @brief Gets/Sets the sort behavior of items in the list
      //! @{
      bool sorted() const { return _sorted; }
      void sorted(bool newval) { 
        _sorted = newval; 
        if (newval) sort();
      }
      //! @}

      //! @brief removes all items
      void clear() noexcept { _items.clear(); ::SendMessage(*this, CB_RESETCONTENT, 0, 0); }

      //! @brief gets the currently selected item in the list
      const typename item::pointer selected_item() const noexcept {
        auto i = ::SendMessage(*this, CB_GETCURSEL, 0, 0);
        return (-1 == i) ? item::pointer() : _items[i];
      }

      //! @brief adds a new item to the list
      //! @{
      void add_item(const item& newval) {
        _items.push_back(std::make_shared<item>(newval));
        if (_sorted) sort();
        else wtf::exception::throw_lasterr_if(::SendMessage(*this, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_items.back()->text().c_str())), [](LRESULT l) { return CB_ERR == l || CB_ERRSPACE == l; });
      }

      void add_item(const tstring& newval) { add_item(item(newval)); }
      //! @}

      //! @brief invoked when the list box of a combo box has been closed
      callback<void(window*)> OnCloseUp;
      //! @brief invoked when the user double-clicks a string in the list box of a combo box
      callback<void(window*)> OnDblClick;
      //! @brief invoked when the list box of a combo box is about to be made visible
      callback<void(window*)> OnDropDown;
      //! @brief invoked after the user has taken an action that may have altered the text in the edit control portion of a combo box
      callback<void(window*)> OnEditChange;
      //! @brief invoked when the edit control portion of a combo box is about to display altered text
      callback<void(window*)> OnEditUpdate;
      //! @brief invoked when a combo box loses the keyboard focus
      callback<void(window*)> OnKillFocus;
      //! @brief invoked when the user changes the current selection in the list box of a combo box
      callback<void(window*)> OnSelChange;
      //! @brief invoked when a combo box receives the keyboard focus
      callback<void(window*)> OnSetFocus;


    protected:
      template <typename, template <typename> typename...> friend struct window_impl;
      void add(window &) override {}

      //! @brief sorts the items in the list
      void sort() {
        std::sort(_items.begin(), _items.end());
        ::SendMessage(*this, CB_RESETCONTENT, 0, 0);
        for (const auto & oItem : _items) {
          wtf::exception::throw_lasterr_if(::SendMessage(*this, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(oItem->text().c_str())), [](LRESULT l) { return CB_ERR == l || CB_ERRSPACE == l; });
        }
      }

#if WTF_USE_COMMON_CONTROLS
      static constexpr TCHAR sub_window_class_name[] = WC_COMBOBOX;
#else
      static constexpr TCHAR sub_window_class_name[] = _T("COMBOBOX");
#endif
      static constexpr TCHAR window_class_name[] = _T("wtf_combobox");
      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;

      void on_wm_command(WPARAM wparam, LPARAM lparam) override {
        switch (HIWORD(wparam)) {
          case CBN_CLOSEUP: OnCloseUp(this); break;
          case CBN_DBLCLK: OnDblClick(this); break;
          case CBN_DROPDOWN: OnDropDown(this); break;
          case CBN_EDITCHANGE: OnEditChange(this); break;
          case CBN_EDITUPDATE: OnEditUpdate(this); break;
          case CBN_KILLFOCUS: OnKillFocus(this); break;
          case CBN_SELCHANGE: OnSelChange(this); break;
          case CBN_SETFOCUS: OnSelChange(this); break;
        }
        __super::on_wm_command(wparam, lparam);
      }

      typename item::vector _items;
      bool _sorted = false;
    };

  }
}

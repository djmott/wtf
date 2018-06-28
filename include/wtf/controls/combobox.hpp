/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_COMBOBOX_SUPER \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_ENABLE \
  DOXY_INHERIT_HAS_MOVE \
  DOXY_INHERIT_HAS_STYLE \
  DOXY_INHERIT_WM_COMMAND


namespace wtf {

#if !DOXY_INVOKED
  namespace _ {
#if WTF_USE_COMMON_CONTROLS
    TCHAR sWC_COMBOBOX[] = WC_COMBOBOX;
#else
    TCHAR sWC_COMBOBOX[] = _T("COMBOBOX");
#endif
  }
#endif

  namespace policy {

    /** @brief Behavior policy of a combobox
    @ingroup Policies
    */
    template <typename _impl_t> struct combobox_super_t : DOXY_INHERIT_COMBOBOX_SUPER window_impl < _impl_t,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_style,
      policy::wm_command
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

      //! @brief sorts the items in the list
      void sort() {
        std::sort(_items.begin(), _items.end());
        ::SendMessage(*this, CB_RESETCONTENT, 0, 0);
        for (const auto & oItem : _items) {
          wtf::exception::throw_lasterr_if(::SendMessage(*this, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(oItem->text().c_str())), [](LRESULT l) { return CB_ERR == l || CB_ERRSPACE == l; });
        }
      }


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


    private:
#if !DOXY_INVOKED
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
#endif
    };
  }


  namespace controls {

    /** @class simple_combobox
    @brief a simple combo box
    @details contains an edit box and a list of items that can be selected
    @ingroup Widgets
    @image html simple_combobox.png
    */
    struct simple_combobox : policy::combobox_super_t<simple_combobox> {
      static constexpr DWORD Style = window::Style | CBS_SIMPLE;
    };

    /** @class dropdown_combobox
    @brief a drop-down combo box
    @details contains a drop-down list of items that can be selected and a text box 
    @ingroup Widgets
    @image html dropdown_combobox.png
    */
    struct dropdown_combobox : policy::combobox_super_t<dropdown_combobox> {
      static constexpr DWORD Style = window::Style | CBS_DROPDOWN;
    };

    /** @class dropdown_list_combobox
    @brief a drop-down list combo box
    @details contains and drop-down list of items that can be selected and displays the current selection in a read-only label
    @ingroup Widgets
    @image html dropdown_list_combobox.png
    */
    struct dropdown_list_combobox : policy::combobox_super_t<dropdown_list_combobox> {
      static constexpr DWORD Style = window::Style | CBS_DROPDOWNLIST;
    };

  }

#if !DOXY_INVOKED
  template <WNDPROC window_proc> struct window_class<controls::simple_combobox, window_proc>
    : super_window_class<_::sWC_COMBOBOX, controls::simple_combobox, window_proc> {};

  template <WNDPROC window_proc> struct window_class<controls::dropdown_combobox, window_proc>
    : super_window_class<_::sWC_COMBOBOX, controls::dropdown_combobox, window_proc> {};

  template <WNDPROC window_proc> struct window_class<controls::dropdown_list_combobox, window_proc>
    : super_window_class<_::sWC_COMBOBOX, controls::dropdown_list_combobox, window_proc> {};
#endif


}

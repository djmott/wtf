/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {


    /** @class listview
    Shows a list of items as text and/or icons
     */
    struct listview : window_impl<listview,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_exstyle,
      policy::has_style,
      policy::wm_command,
      policy::wm_notify, 
      policy::wm_showwindow
    > {
      struct item;
      struct subitem;
      struct column;

      static constexpr DWORD Style = window::Style | LVS_REPORT | LVS_EDITLABELS | LVS_AUTOARRANGE | LVS_SORTASCENDING;

      enum class styles {
        icon = LVS_ICON,
        small_icon = LVS_SMALLICON,
        list = LVS_LIST,
        report = LVS_REPORT,
      };




      /** @class column
      represents a columns in the list view
       */
      struct column : LVCOLUMN {
        using pointer = std::shared_ptr<column>;

        column() = delete;
        column(const column&) = delete;
        column& operator=(const column&) = delete;
        column(column&& src) : _parent(std::move(src._parent)), _text(std::move(src._text)) {}
        column& operator=(column&& src) {
          std::swap(_parent, src._parent);
          std::swap(_text, src._text);
          return *this;
        }


        /** @class collection
        maintains a collection of columns
        */
        struct collection {
          collection() = delete;
          collection(const collection&) = delete;
          collection& operator=(const collection&) = delete;
          collection(collection&& src) : _parent(std::move(src._parent)), _inner(std::move(src._inner)) {}
          collection& operator=(collection&& src) {
            std::swap(_parent, src._parent);
            std::swap(_inner, src._inner);
            return *this;
          }

          size_t size() const { return _inner.size(); }

          typename column::pointer operator[](size_t index) { return _inner[index]; }
          const typename column::pointer operator[](size_t index) const { return _inner[index]; }

          column::pointer add(const tstring& sText){
            typename column::pointer oRet(new column(_parent, sText));
            wtf::exception::throw_lasterr_if(::SendMessage(*_parent, LVM_INSERTCOLUMN, _inner.size(), reinterpret_cast<LPARAM>(oRet.get())), [](LRESULT l) {return -1 == l; });
            _inner.push_back(oRet);
            return oRet;
          }

        protected:
          friend struct column;
          friend struct listview;
          collection(listview* pListview) : _parent(pListview){}

          listview * _parent;
          std::vector<typename column::pointer> _inner;

        };

      private:
        friend struct listview;
        column(listview * parent, text_horizontal_alignments alignment, const tstring& sText, int width, int minwidth) : _parent(parent), _text(sText){
          memset(this, 0, sizeof(LVCOLUMN));
          mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_MINWIDTH;
          switch (alignment) {
            case text_horizontal_alignments::center: fmt = LVCFMT_CENTER; break;
            case text_horizontal_alignments::left: fmt = LVCFMT_LEFT; break;
            case text_horizontal_alignments::right: fmt = LVCFMT_RIGHT; break;
          }
          pszText = &_text[0];
          cx = width;
          cxMin = minwidth;
        }
        column(listview * parent, const tstring& sText) : column(parent, text_horizontal_alignments::left, sText, 100, 100){}
        listview * _parent;
        tstring _text;
      };


      /** @class subitem
      represents a subitem of an item
      */
      struct subitem : LVITEM {
        using pointer = std::shared_ptr<subitem>;
        subitem() = delete;
        subitem(const subitem&) = delete;
        subitem& operator=(const subitem&) = delete;
        subitem(subitem&& src) : _parent(std::move(src._parent)), _item(std::move(src._item)), _text(std::move(src._text)) {}
        subitem& operator=(subitem&& src) {
          std::swap(_parent, src._parent);
          std::swap(_item, src._item);
          std::swap(_text, src._text);
          return *this;
        }

        /** @class collection
        represents a collection of subitems
        */
        struct collection {
          collection() = delete;
          collection(const collection&) = delete;
          collection& operator=(const collection&) = delete;
          collection(collection&& src) : _parent(std::move(src._parent)), _item(std::move(src._item)), _inner(std::move(src._inner)) {}
          collection& operator=(collection&& src) {
            std::swap(_parent, src._parent);
            std::swap(_item, src._item);
            std::swap(_inner, src._inner);
            return *this;
          }

          size_t size() const { return _inner.size(); }

          subitem& operator[](size_t index) { return *_inner[index]; }
          const subitem& operator[](size_t index) const { return *_inner[index]; }

          typename subitem::pointer add(const tstring& Text) {
            typename subitem::pointer oRet(new subitem(_parent, _item, Text));
            wtf::exception::throw_lasterr_if(::SendMessage(*_parent, LVM_SETITEM, 0, reinterpret_cast<LPARAM>(static_cast<LVITEM*>(oRet.get()))), [](LRESULT l) { return -1 == l; });
            _inner.push_back(oRet);
            return oRet;
          }

        protected:
          friend struct item;
          friend struct listview;
          listview * _parent;
          item * _item;
          std::vector<typename subitem::pointer> _inner;

          collection(listview* pListview, item * pItem) : _parent(pListview), _item(pItem) {}

        };

      protected:
        friend struct listview;
        friend struct collection;
        friend struct listview::item;
        listview * _parent;
        item * _item;
        tstring _text;

        subitem(listview * pParent, item * pItem, const tstring& sText) : _parent(pParent), _item(pItem), _text(sText){
          memset(this, 0, sizeof(LVITEM));
          iItem = pItem->index();
          mask = LVIF_TEXT;
          iSubItem = static_cast<int>(1 + pItem->_subitems.size());
          pszText = &_text[0];
          lParam = reinterpret_cast<LPARAM>(this);
        }

      };


      /** @class item
      represents a selectable item in the list view
       */
      struct item : LVITEM {
        using pointer = std::shared_ptr<item>;

        item() = delete;
        item(const item&) = delete;
        item& operator=(const item&) = delete;
        item(item&& src) : _parent(std::move(src._parent)), _subitems(std::move(src._subitems)), _text(std::move(src._text)), _id(std::move(src._id)) {}
        item& operator=(item&& src) {
          std::swap(_parent, src._parent);
          std::swap(_subitems, src._subitems);
          std::swap(_text, src._text);
          std::swap(_id, src._id);
          return *this;
        }

        /** @class collection
        maintains a collection of items and its subitems
        */
        struct collection  {

          collection() = delete;
          collection(const collection&) = delete;
          collection& operator=(const collection&) = delete;
          collection(collection&& src) : _parent(std::move(src._parent)), _item(std::move(src._item)), _inner(std::move(src._inner)) {}
          collection& operator=(collection&& src) {
            std::swap(_parent, src._parent);
            std::swap(_item, src._item);
            std::swap(_inner, src._inner);
            return *this;
          }


          size_t size() const { return _inner.size(); }

          typename item::pointer add(const tstring& Text) {
            typename item::pointer oRet(new item(_parent, _item, Text));
            wtf::exception::throw_lasterr_if(::SendMessage(*_parent, LVM_INSERTITEM, 0, reinterpret_cast<LPARAM>(static_cast<LVITEM*>(oRet.get()))), [](LRESULT l) { return -1 == l; });
            oRet->_id = ::SendMessage(*_parent, LVM_MAPINDEXTOID, oRet->iItem, 0);
            _inner.push_back(oRet);
            return oRet;
          }

        private:
          friend struct listview;
          friend struct item;
          listview * _parent;
          item * _item;
          std::vector<typename item::pointer> _inner;

          collection(listview * pParent, item * pItem) : _parent(pParent), _item(pItem) {}

        };

        subitem::collection& subitems() { return _subitems; }
        const subitem::collection& subitems() const { return _subitems; }

        int index() const { return static_cast<int>(::SendMessage(*_parent, LVM_MAPIDTOINDEX, _id, 0)); }

        bool is_visible() const { return (TRUE==::SendMessage(*_parent, LVM_ISITEMVISIBLE, index(), 0) ? true : false); }

      private:
        friend struct listview;
        friend struct collection;
        listview * _parent;
        subitem::collection _subitems;
        tstring _text;
        LRESULT _id;

        item(listview * pParent, item *, const tstring& Text) : _parent(pParent), _subitems(pParent, this), _text(Text), _id(-1){
          memset(this, 0, sizeof(LVITEM));
          mask = LVIF_TEXT | LVIF_PARAM;
          pszText = &_text[0];
          lParam = reinterpret_cast<LPARAM>(this);
        }
      
      };

      listview() : _items(this, nullptr), _columns(this) {}

      styles style() const {
        auto iStyle = wtf::exception::throw_lasterr_if(::GetWindowLong(*this, GWL_STYLE), [](LONG l) { return !l; });
        if (LVS_ICON & iStyle) return styles::icon;
        if (LVS_SMALLICON & iStyle) return styles::small_icon;
        if (LVS_LIST & iStyle) return styles::list;
        return styles::report;
      }
      void style(styles newval) {
        auto iStyle = wtf::exception::throw_lasterr_if(::GetWindowLong(*this, GWL_STYLE), [](LONG l) { return !l; });
        iStyle &= ~LVS_TYPEMASK;
        iStyle |= static_cast<DWORD>(newval);
        ::SetWindowLong(*this, GWL_STYLE, iStyle);
      }

      bool auto_arrange() const { return get_style_bit<LVS_AUTOARRANGE>(); }
      void auto_arrange(bool newval) { set_style_bit<LVS_AUTOARRANGE>(newval); }

      bool hide_headers() const { return get_style_bit<LVS_NOCOLUMNHEADER>(); }
      void hide_headers(bool newval) { set_style_bit<LVS_NOCOLUMNHEADER>(newval); }

      bool single_select() const { return get_style_bit<LVS_SINGLESEL>(); }
      void single_select(bool newval) { return set_style_bit<LVS_SINGLESEL>(newval); }

      bool row_select() const { return get_exstyle<LVS_EX_FULLROWSELECT>(); }
      void row_select(bool newval) { set_exstyle<LVS_EX_FULLROWSELECT>(newval); }

      bool grid_lines () const { return get_exstyle<LVS_EX_GRIDLINES>(); }
      void grid_lines(bool newval) { set_exstyle<LVS_EX_GRIDLINES>(newval); }

      column::collection& columns() { return _columns; }
      const column::collection& columns() const { return _columns; }

      item::collection& items() { return _items; }
      const item::collection& items() const { return _items; }

      callback<void(window*, typename column::pointer col)> OnColumnClick;
      callback<void(window*, typename item::pointer item)> OnItemClick;

    protected:
      item::collection _items;
      column::collection _columns;

      virtual void on_column_click(const NMLISTVIEW * lv) { OnColumnClick(this, _columns[lv->iSubItem]); }

      virtual void on_click(const NMITEMACTIVATE* item) {
        auto iID = index_2_id(item->iItem);
        auto oItem = std::find_if(_items._inner.begin(), _items._inner.end(), [iID](const typename item::pointer& item) { return iID == item->_id; });
        if (_items._inner.end() != oItem) OnItemClick(this, *oItem);
      }

      void on_wm_notify(NMHDR * notification) override{
        switch (notification->code) {
          case LVN_COLUMNCLICK: on_column_click(reinterpret_cast<NMLISTVIEW*>(notification)); break;
          case NM_CLICK: on_click(reinterpret_cast<NMITEMACTIVATE*>(notification)); break;
        }
      }

      LRESULT index_2_id(int index) const { return ::SendMessage(*this, LVM_MAPINDEXTOID, index, 0); }


    };

  }

  namespace _ { TCHAR sWC_LISTVIEW[] = WC_LISTVIEW; }

  template <WNDPROC window_proc> struct window_class<controls::listview, window_proc> : super_window_class<_::sWC_LISTVIEW, controls::listview, window_proc> {};

}

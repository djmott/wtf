/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ { 
      
      TCHAR sWC_TREEVIEW[] = WC_TREEVIEW;

      template <typename _impl_t> using tree_impl = window_impl<_impl_t,
        policy::has_font,
        wtf::policy::has_move,
        wtf::policy::wm_notify
      >;

    }

    /** @class tree
    @ingroup Widgets
    @brief A tree-view control is a window that displays a hierarchical list of items, such as the headings in a document, the entries in an index, or the files and directories on a disk.
    */
    struct tree : _::tree_impl<tree> {
      tree() : _::tree_impl<tree>() {
        wtf::_::init_common_controls<wtf::_::treeview_classes>::get();
      }

      struct item : TVITEMEX {
        using pointer = std::shared_ptr<item>;
        using weak_ptr = std::weak_ptr<item>;
        using vector = std::vector<pointer>;

        item(weak_ptr parent, const tstring& text) : _parent(parent), _text(text) {
          memset(this, 0, sizeof(TVITEMEX));
          lParam = reinterpret_cast<LPARAM>(this);
          pszText = &_text[0];
          cChildren = I_CHILDRENCALLBACK;
          mask = TVIF_CHILDREN | TVIF_TEXT | TVIF_PARAM;
        }

        pointer parent() const noexcept { return _parent.lock(); }
        const vector& children() const noexcept { return _children; }

        const tstring& text() const noexcept { return _text; }
      protected:
        friend struct tree;
        weak_ptr _parent;
        vector _children;
        tstring _text;
      };

      callback<void(window*)> OnClick;
      callback<void(window*)> OnDblClick;
      callback<void(window*)> OnBeginEdit;
      callback<void(window*)> OnEndEdit;

      typename item::pointer add_item(const tstring& text) {
        auto oRet = std::make_shared<item>(typename item::weak_ptr(), text);
        TVINSERTSTRUCT oInsert;
        memset(&oInsert, 0, sizeof(TVINSERTSTRUCT));
        oInsert.hInsertAfter = TVI_ROOT;
        memcpy(&oInsert.itemex, oRet.get(), sizeof(TVITEMEX));
        oRet->hItem = wtf::exception::throw_lasterr_if(reinterpret_cast<HTREEITEM>(::SendMessage(*this, TVM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&oInsert))), [](HTREEITEM h) { return nullptr == h; });
        _items.push_back(oRet);
        return oRet;
      }

      typename item::pointer add_item(typename item::pointer parent, const tstring& text) {
        auto oRet = std::make_shared<item>(parent, text);
        TVINSERTSTRUCT oInsert;
        memset(&oInsert, 0, sizeof(TVINSERTSTRUCT));
        if (parent) {
          oInsert.hParent = parent->hItem;
          parent->_children.push_back(oRet);
        }
        oInsert.hInsertAfter = TVI_LAST;
        memcpy(&oInsert.itemex, oRet.get(), sizeof(TVITEMEX));
        oRet->hItem = wtf::exception::throw_lasterr_if(reinterpret_cast<HTREEITEM>(::SendMessage(*this, TVM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&oInsert))), [](HTREEITEM h) { return nullptr == h; });
        _items.push_back(oRet);
        return oRet;
      }

    protected:

      template <typename, template <typename> typename...> friend struct window_impl;

      typename item::vector _items;

      void get_display_info(NMTVDISPINFO * pInfo){
        const auto & oItem = std::find_if(_items.cbegin(), _items.cend(), [&pInfo](const typename item::pointer& it)->bool { return pInfo->item.hItem == it->hItem; });
        if (_items.cend() == oItem) return;
        if (pInfo->item.mask & TVIF_CHILDREN) pInfo->item.cChildren = static_cast<int>((*oItem)->children().size());
        if (pInfo->item.mask & TVIF_HANDLE) pInfo->item.hItem = (*oItem)->hItem;
        if (pInfo->item.mask & TVIF_TEXT) pInfo->item.pszText = const_cast<TCHAR*>((*oItem)->text().c_str());
      }

      void on_wm_notify(NMHDR * notification) override{
        if (TVN_GETDISPINFO == notification->code) get_display_info(reinterpret_cast<NMTVDISPINFO*>(notification));
        else if (NM_CLICK == notification->code) OnClick(this);
        else if (NM_DBLCLK == notification->code) OnDblClick(this);
        else if (TVN_BEGINLABELEDIT == notification->code) OnBeginEdit(this);
        else if (TVN_ENDLABELEDIT == notification->code) OnEndEdit(this);
        _::tree_impl<tree>::on_wm_notify(notification);
      }

    };

  }

  template <WNDPROC window_proc> struct window_class<controls::tree, window_proc> : super_window_class<controls::_::sWC_TREEVIEW, controls::tree, window_proc> {};

}


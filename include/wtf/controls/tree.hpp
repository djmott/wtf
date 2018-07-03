/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_TREE_SUPER \
  DOXY_INHERIT_WINDOW \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_MOVE \
  DOXY_INHERIT_NM_KILLFOCUS \
  DOXY_INHERIT_NM_SETFOCUS \
  DOXY_INHERIT_NM_CLICK \
  DOXY_INHERIT_NM_RCLICK \
  DOXY_INHERIT_NM_DBLCLICK \
  DOXY_INHERIT_WM_NOTIFY 



namespace wtf {
  namespace controls {


    /** @class tree
    @brief A tree-view control is a window that displays a hierarchical list of items, such as the headings in a document, the entries in an index, or the files and directories on a disk.
    @ingroup Controls
    */
    struct tree : DOXY_INHERIT_TREE_SUPER window_impl<tree,
      policy::has_font,
      policy::has_move,
      messages::nm_killfocus,
      messages::nm_setfocus,
      messages::nm_click,
      messages::nm_rclick,
      messages::nm_dblclick,
      messages::wm_notify
    > {


      tree() : _items(this, nullptr){}

      callback<void(window*)> OnBeginEdit;
      callback<void(window*)> OnEndEdit;

      /** @class item
      represents a node in the tree
      */
      struct item : TVITEMEX {
        using pointer = std::shared_ptr<item>;


        /** @class collection
        Maintains a collection of tree::item
         */
        struct collection {
          size_t size() const { return _inner.size(); }
          typename item::pointer& operator[](int index) { return _inner[index]; }
          const typename item::pointer& operator[](int index) const { return _inner[index]; }

          typename item::pointer add(const tstring& text) {
            typename item::pointer oRet(new item(_tree, text));
            TVINSERTSTRUCT oInsert;
            memset(&oInsert, 0, sizeof(TVINSERTSTRUCT));
            oInsert.hParent = (_parent ? _parent->hItem : nullptr);
            oInsert.hInsertAfter = (nullptr == _parent ? TVI_ROOT : TVI_LAST);
            memcpy(&oInsert.itemex, oRet.get(), sizeof(TVITEMEX));
            oRet->hItem = wtf::exception::throw_lasterr_if(reinterpret_cast<HTREEITEM>(::SendMessage(*_tree, TVM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&oInsert))), [](HTREEITEM h) { return nullptr == h; });
            _inner.push_back(oRet);
            return oRet;
          }

        private:
          friend struct item;
          friend struct tree;
          using vector = std::vector<typename item::pointer>;
          collection(tree * pTree, item * pParent) : _tree(pTree), _parent(pParent){}
          vector _inner;
          tree * _tree;
          item * _parent;
        };


        const tstring& text() const noexcept { return _text; }

        item::collection& items() { return _items; }
        const item::collection& items() const { return _items; }

      protected:
        friend struct tree;

        item(tree * pTree, const tstring& text) : _tree(pTree), _text(text), _items(pTree, this) {
          memset(this, 0, sizeof(TVITEMEX));
          lParam = reinterpret_cast<LPARAM>(this);
          pszText = &_text[0];
          cChildren = I_CHILDRENCALLBACK;
          mask = TVIF_CHILDREN | TVIF_TEXT | TVIF_PARAM;
        }

        tree * _tree;
        tstring _text;
        collection _items;
      };

      item::collection& items() { return _items; }
      const item::collection& items() const { return _items; }

    protected:

      template <typename, template <typename> typename...> friend struct window_impl;
      static constexpr TCHAR sub_window_class_name[] = WC_TREEVIEW;
      static constexpr TCHAR window_class_name[] = _T("wtf_tree");
      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;

      item::collection _items;

      void get_display_info(NMTVDISPINFO * pInfo){
        const auto & oItem = std::find_if(_items._inner.cbegin(), _items._inner.cend(), [&pInfo](const typename item::pointer& it)->bool { return pInfo->item.hItem == it->hItem; });
        if (_items._inner.cend() == oItem) return;
        if (pInfo->item.mask & TVIF_CHILDREN) pInfo->item.cChildren = static_cast<int>((*oItem)->items().size());
        if (pInfo->item.mask & TVIF_HANDLE) pInfo->item.hItem = (*oItem)->hItem;
        if (pInfo->item.mask & TVIF_TEXT) pInfo->item.pszText = const_cast<TCHAR*>((*oItem)->text().c_str());
      }

      void on_wm_notify(NMHDR * notification) override{
        if (TVN_GETDISPINFO == notification->code) get_display_info(reinterpret_cast<NMTVDISPINFO*>(notification));
        else if (TVN_BEGINLABELEDIT == notification->code) OnBeginEdit(this);
        else if (TVN_ENDLABELEDIT == notification->code) OnEndEdit(this);
        __super::on_wm_notify(notification);
      }

    };

  }

}


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
      tree() : _::tree_impl<tree>(), _items(this, nullptr) {
        wtf::_::init_common_controls<wtf::_::treeview_classes>::get();
      }


      callback<void(window*)> OnClick;
      callback<void(window*)> OnDblClick;
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


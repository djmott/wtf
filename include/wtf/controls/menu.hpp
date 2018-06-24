/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {
    
    struct menu_item : MENUITEMINFO {
      ~menu_item() = default;
      menu_item() = delete;
      menu_item(const menu_item&) = delete;
      menu_item& operator=(const menu_item&) = delete;
      menu_item(const tstring& sText) : _text(sText) {
        memset(this, 0, sizeof(MENUITEMINFO));
        cbSize = sizeof(MENUITEMINFO);
        fMask = MIIM_DATA | MIIM_ID | MIIM_STRING | MIIM_FTYPE;
        fType = MFT_STRING;
        wID = gen_id::get();
        dwItemData = reinterpret_cast<ULONG_PTR>(this);
        dwTypeData = &_text[0];
      }
      callback<void(menu_item*)> OnClick;
    protected:
      friend struct menu_bar;
      friend struct popup_menu;
      tstring _text;

      struct gen_id {
        static UINT get() {
          static std::atomic<UINT> _genid;
          return ++_genid;
        }
      };

    };

    struct popup_menu : menu_item, window{
      popup_menu() = delete;
      ~popup_menu() override { DestroyMenu(_hmenu); }
      popup_menu(const tstring& sText) : menu_item(sText){
        fMask |= MIIM_SUBMENU;
      }

      const std::type_info& type() const noexcept override { return typeid(popup_menu); }

      operator HMENU() const noexcept { return _hmenu; }

      int run() override {
        if (_hmenu) return 0;
        _hmenu = wtf::exception::throw_lasterr_if(::CreatePopupMenu(), [](HMENU h) { return nullptr == h; });
        hSubMenu = _hmenu;
        for (auto & oItem : _items) {
          wtf::exception::throw_lasterr_if(::InsertMenuItem(_hmenu, oItem->wID, FALSE, oItem), [](BOOL b) { return !b; });
        }
        return 0;
      }
      void add(menu_item& oMenu) {
        _items.push_back(&oMenu);
        if (_hmenu) {
          wtf::exception::throw_lasterr_if(::InsertMenuItem(_hmenu, oMenu.wID, FALSE, &oMenu), [](BOOL b) { return !b; });
        }
      }
    protected:
      HMENU _hmenu = nullptr;
      std::vector<menu_item*> _items;

      void handle_msg(wtf::window_message& msg) override {
      }

      void fwd_msg(wtf::window_message&, const std::type_info&) override {}

    };

    struct menu_bar : window {

      menu_bar() = default;
      ~menu_bar() override { DestroyMenu(_hmenu); }

      const std::type_info& type() const noexcept override { return typeid(menu_bar); }

      operator HMENU() const noexcept { return _hmenu; }

      int run() override {
        if (_hmenu) return 0;
        _hmenu = wtf::exception::throw_lasterr_if(::CreateMenu(), [](HMENU h) { return nullptr == h; });
        for (auto & oItem : _items) {
          oItem->run();
          wtf::exception::throw_lasterr_if(::InsertMenuItem(_hmenu, oItem->wID, FALSE, oItem), [](BOOL b) { return !b; });
        }
        wtf::exception::throw_lasterr_if(::SetMenu(*parent(), *this), [](BOOL b) { return !b; });
        wtf::exception::throw_lasterr_if(::DrawMenuBar(*parent()), [](BOOL b) { return !b; });
        OnCreated(this);
        SetWindowPos(*parent(), 0, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
        return 0;
      }

      void add(popup_menu& oMenu) {
        _items.push_back(&oMenu);
        if (_hmenu) {
          oMenu.run();
          wtf::exception::throw_lasterr_if(::InsertMenuItem(_hmenu, oMenu.wID, FALSE, &oMenu), [](BOOL b) { return !b; });
        }
      }

    protected:
      HMENU _hmenu = nullptr;
      std::vector<popup_menu*> _items;

      void handle_msg(wtf::window_message& msg) override {
        if (WM_COMMAND != msg.umsg) return;
        if (0 != HIWORD(msg.wparam)) return;
        MENUITEMINFO mii;
        memset(&mii, 0, sizeof(MENUITEMINFO));
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_DATA;
        wtf::exception::throw_lasterr_if(::GetMenuItemInfo(_hmenu, LOWORD(msg.wparam), FALSE, &mii), [](BOOL b) { return !b; });
        if (!mii.dwItemData) return;
        auto pMenuItem = reinterpret_cast<menu_item*>(mii.dwItemData);
        pMenuItem->OnClick(pMenuItem);
      }

      void fwd_msg(wtf::window_message&, const std::type_info&) override {}

    };


  }

}

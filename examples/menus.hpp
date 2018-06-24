/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

struct mnuMain : menu_bar {

  mnuMain() {
    add(mnuFile);
    add(mnuEdit);
  }

  struct mnuFile : popup_menu {
    mnuFile() : popup_menu(_T("File")) {
      add(mnuOpen);
      add(mnuNew);
      add(mnuExit);
    }

    struct mnuOpen : menu_item {
      mnuOpen() : menu_item(_T("Open")) {
        OnClick += [this](...) {
        };
      }
    }mnuOpen;

    struct mnuNew : menu_item {
      mnuNew() : menu_item(_T("New")) {
        OnClick += [this](...) {
        };
      }
    }mnuNew;

    struct mnuExit : menu_item {
      mnuExit() : menu_item(_T("Exit")) {
        OnClick += [this](...) {
        };
      }
    }mnuExit;

  }mnuFile;


  struct mnuEdit : popup_menu {
    mnuEdit() : popup_menu(_T("Edit")) {
      add(mnuCopy);
      add(mnuCut);
      add(mnuPaste);
    }

    struct mnuCopy : menu_item {
      mnuCopy() : menu_item(_T("Copy")) {
        OnClick += [this](...) {
        };
      }
    }mnuCopy;

    struct mnuCut : menu_item {
      mnuCut() : menu_item(_T("Cut")) {
        OnClick += [this](...) {
        };
      }
    }mnuCut;

    struct mnuPaste : menu_item {
      mnuPaste() : menu_item(_T("Paste")) {
        OnClick += [this](...) {
        };
      }
    }mnuPaste;

  }mnuEdit;

};
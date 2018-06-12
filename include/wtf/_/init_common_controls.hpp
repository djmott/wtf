/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf {
  namespace _ {
    struct init_common_controls : INITCOMMONCONTROLSEX {
      static init_common_controls& get() {
        static init_common_controls _init_common_controls;
        return _init_common_controls;
      }
    private:
      init_common_controls() {
        dwSize = sizeof(INITCOMMONCONTROLSEX);
        dwICC = ICC_LISTVIEW_CLASSES |
          ICC_TREEVIEW_CLASSES |
          ICC_BAR_CLASSES |
          ICC_TAB_CLASSES |
          ICC_UPDOWN_CLASS |
          ICC_PROGRESS_CLASS |
          ICC_HOTKEY_CLASS |
          ICC_ANIMATE_CLASS |
          ICC_WIN95_CLASSES |
          ICC_DATE_CLASSES |
          ICC_USEREX_CLASSES |
          ICC_COOL_CLASSES |
          ICC_INTERNET_CLASSES |
          ICC_PAGESCROLLER_CLASS |
          ICC_NATIVEFNTCTL_CLASS |
          ICC_STANDARD_CLASSES |
          ICC_LINK_CLASS;
        wtf::exception::throw_lasterr_if(InitCommonControlsEx(this), [](BOOL b) { return !b; });
      }
    };
  }
}
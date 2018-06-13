/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf {
  namespace _ {

    enum init_common_controls_class_ids : DWORD {
      listview_classes = ICC_LISTVIEW_CLASSES,
      treeview_classes = ICC_TREEVIEW_CLASSES,
      bar_classes = ICC_BAR_CLASSES,
      tab_classes = ICC_TAB_CLASSES,
      updown_classes = ICC_UPDOWN_CLASS,
      progress_classes = ICC_PROGRESS_CLASS,
      hotkey_classes = ICC_HOTKEY_CLASS,
      animate_classes = ICC_ANIMATE_CLASS,
      win95_classes = ICC_WIN95_CLASSES,
      date_classes = ICC_DATE_CLASSES,
      userex_classes = ICC_USEREX_CLASSES,
      cool_classes = ICC_COOL_CLASSES,
      internet_classes = ICC_INTERNET_CLASSES,
      pagescroller_classes = ICC_PAGESCROLLER_CLASS,
      nativefntctl_classes = ICC_NATIVEFNTCTL_CLASS,
      standard_classes = ICC_STANDARD_CLASSES,
      link_classes = ICC_LINK_CLASS,
    };

    template <init_common_controls_class_ids _class_id>
    struct init_common_controls : INITCOMMONCONTROLSEX {
      static init_common_controls& get() {
        static init_common_controls _init_common_controls;
        return _init_common_controls;
      }
    private:
      init_common_controls() {
        dwSize = sizeof(INITCOMMONCONTROLSEX);
        dwICC = _class_id;
        wtf::exception::throw_lasterr_if(InitCommonControlsEx(this), [](BOOL b) { return FALSE==b; });
      }
    };
  }
}
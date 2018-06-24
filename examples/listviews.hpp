/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

struct ListViews : wtf::custom::splitter {

  ListViews() : splitter(_lv1, _lv2){
    add(_lv1);
    add(_lv2);

    OnCreated += [this](...) {
      auto setup = [](wtf::controls::listview& oListview) {
        oListview.columns().add(_T("Name"));
        oListview.columns().add(_T("Phone"));
        oListview.columns().add(_T("Email"));
        auto oItem = oListview.items().add(_T("Homer"));
        oItem->subitems().add(_T("555-555-5555"));
        oItem->subitems().add(_T("homer@springfield"));
        oItem = oListview.items().add(_T("Flanders"));
        oItem->subitems().add(_T("555-555-6666"));
        oItem->subitems().add(_T("ned.flanders@springfield"));
      };
      setup(_lv1);
      setup(_lv2);
    };

    OnShow += [this](...) {
      if (!_init) set_split_relative(50);
    };
  }
  bool _init = false;
  wtf::controls::listview _lv1, _lv2;
};

/*
struct ListViews : wtf::controls::listview {

  ListViews() {


    OnCreated += [this](...) {
      columns().add(_T("Name"));
      columns().add(_T("Phone"));
      columns().add(_T("Email"));
      auto oItem = items().add(_T("Homer"));
      oItem->subitems().add(_T("555-555-5555"));
      oItem->subitems().add(_T("homer@springfield"));
      oItem = items().add(_T("Flanders"));
      oItem->subitems().add(_T("555-555-6666"));
      oItem->subitems().add(_T("ned.flanders@springfield"));
    };
  }
};*/
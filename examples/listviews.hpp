

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
};
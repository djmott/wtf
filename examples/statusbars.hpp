
struct StatusBar : wtf::controls::statusbar {
  StatusBar() {
    OnCreated += [this](...) {
      parts().add(_T("Ready"));
      parts().add(_T("CAPS"));
      parts().add(_T("\t\tNUM       "), -1);
    };
  }
};
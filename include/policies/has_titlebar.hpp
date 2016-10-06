#pragma once

namespace wtf {
  namespace policy {

    /** has_titlebar
    * Adds titlebar text features to windows
    */
    template<typename _SuperT, typename _ImplT>
    struct has_titlebar : _SuperT {
      has_titlebar() = default;
      virtual ~has_titlebar() = default;
      has_titlebar(const has_titlebar&) = delete;
      has_titlebar &operator=(const has_titlebar &) = delete;
      has_titlebar(has_titlebar&&) = delete;
      has_titlebar &operator=(has_titlebar&&) = delete;


      const tstring &titlebar() const {
        return _titlebar;
      }

      void titlebar(LPCTSTR newval) {
        wtf::exception::throw_lasterr_if(::SetWindowText(*this, newval), [](BOOL b) { return !b; });
        _titlebar = newval;
      }

    protected:

      tstring _titlebar;
    };

  }
}

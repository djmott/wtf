/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace dialogs {
    struct file_open  {
      ~file_open() = default;
      file_open& operator=(const file_open&) = delete;
      file_open(const file_open&) = delete;
      file_open() : _file(0x400, _T('\0')){}

      tstring& filter() noexcept { return _filter; }
      const tstring& filter() const noexcept { return _filter; }

      tstring& file() noexcept { return _file; }
      const tstring& file() const noexcept { return _file; }

      const std::vector<tstring>& files() const noexcept { return _files; }

      tstring& initial_directory() noexcept { return _initial_dir; }
      const tstring& initial_directory() const noexcept { return _initial_dir; }

      tstring& title() noexcept { return _title; }
      const tstring& title() const noexcept { return _title; }

      bool multi_select() const noexcept { return _flags & OFN_ALLOWMULTISELECT; }
      void multi_select(bool newval) noexcept{
        _flags &= ~OFN_ALLOWMULTISELECT;
        _flags |= (newval ? OFN_ALLOWMULTISELECT : 0);
      }

      bool allow_create() const noexcept { return _flags & OFN_CREATEPROMPT; }
      void allow_create(bool newval) noexcept {
        _flags &= ~OFN_CREATEPROMPT;
        _flags |= (newval ? OFN_CREATEPROMPT : 0);
      }

      bool add_to_recent() const noexcept { return !(_flags & OFN_DONTADDTORECENT); }
      void add_to_recent(bool newval) noexcept {
        _flags &= ~OFN_DONTADDTORECENT;
        _flags |= (newval ? 0 : OFN_DONTADDTORECENT);
      }

      bool sizeable() const noexcept { return _flags & OFN_ENABLESIZING; }
      void sizeable(bool newval) noexcept {
        _flags &= ~OFN_ENABLESIZING;
        _flags |= (newval ? OFN_ENABLESIZING : 0);
      }

      bool file_must_exist() const noexcept { return _flags & OFN_FILEMUSTEXIST; }
      void file_must_exist(bool newval) noexcept {
        _flags &= ~OFN_FILEMUSTEXIST;
        _flags |= (newval ? OFN_FILEMUSTEXIST : 0);
      }

      bool show_hidden() const noexcept { return _flags & OFN_FORCESHOWHIDDEN; }
      void show_hidden(bool newval) noexcept {
        _flags &= ~OFN_FORCESHOWHIDDEN;
        _flags |= (newval ? OFN_FORCESHOWHIDDEN : 0);
      }

      bool readonly_checkbox() const noexcept { return !(_flags & OFN_HIDEREADONLY); }
      void readonly_checkbox(bool newval) noexcept {
        _flags &= ~OFN_HIDEREADONLY;
        _flags |= (newval ? 0 : OFN_HIDEREADONLY);
      }

      bool readonly_checked() const noexcept { return !(_flags & OFN_READONLY); }
      void readonly_checked(bool newval) noexcept {
        _flags &= ~OFN_READONLY;
        _flags |= (newval ? OFN_READONLY : 0);
      }

      bool change_directory() const noexcept { return !(_flags & OFN_NOCHANGEDIR); }
      void change_directory(bool newval) noexcept {
        _flags &= ~OFN_NOCHANGEDIR;
        _flags |= (newval ? 0 : OFN_NOCHANGEDIR);
      }

      bool long_file_names() const noexcept { return (_flags & OFN_LONGNAMES); }
      void long_file_names(bool newval) noexcept {
        _flags &= ~(OFN_LONGNAMES | OFN_NOLONGNAMES);
        _flags |= (newval ? OFN_LONGNAMES : OFN_NOLONGNAMES);
      }

      bool network_button() const noexcept { return !(_flags & OFN_NONETWORKBUTTON); }
      void network_button(bool newval) noexcept {
        _flags &= ~OFN_NONETWORKBUTTON;
        _flags |= (newval ? 0 : OFN_NONETWORKBUTTON);
      }

      bool no_readonly() const noexcept { return _flags & OFN_NOREADONLYRETURN; }
      void no_readonly(bool newval) noexcept {
        _flags &= ~OFN_NOREADONLYRETURN;
        _flags |= (newval ? OFN_NOREADONLYRETURN : 0);
      }

      bool path_must_exist() const noexcept { return _flags & OFN_PATHMUSTEXIST; }
      void path_must_exist(bool newval) noexcept {
        _flags &= ~OFN_PATHMUSTEXIST;
        _flags |= (newval ? OFN_PATHMUSTEXIST : 0);
      }



      bool run(window * owner) { return _run<::GetOpenFileName>(owner); }

    protected:
      DWORD _flags = (OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST);
      tstring _filter=_T("All Files (*.*)|*.*");
      tstring _file;
      tstring _initial_dir = _T("");
      tstring _title = _T("Open File");
      std::vector<tstring> _files;


      template <BOOL (*dlgfn)(OPENFILENAME*)>
      bool _run(window * owner) {
        OPENFILENAME ofn;
        memset(&ofn, 0, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = (owner ? owner->handle() : nullptr);
        tstring sFilter = _filter;
        sFilter.push_back(0);
        std::replace(sFilter.begin(), sFilter.end(), _T('|'), _T('\0'));
        ofn.lpstrFile = &_file[0];
        ofn.nMaxFile = static_cast<DWORD>(1 + _file.size());
        ofn.lpstrFilter = &sFilter[0];
        ofn.lpstrInitialDir = &_initial_dir[0];
        ofn.lpstrTitle = &_title[0];
        ofn.nMaxFileTitle = static_cast<DWORD>(1 + _title.size());
        ofn.Flags = _flags;
        if (!dlgfn(&ofn)) return false;
        if (!multi_select()) {
          _files.push_back(_file);
          return true;
        }
        auto f = 1 + _file.find(_T('\0'));
        for (auto l = _file.find(_T('\0'), f); l < _file.size() && l >= f && _file[f] != _T('\0'); f = ++l, l = _file.find(_T('\0'), f)) {
          _files.push_back(_file.substr(f, l - f));
        }
        return true;
      }
    };

    struct file_save : file_open {
      ~file_save() = default;
      file_save(const file_save&) = delete;
      file_save& operator=(const file_save&) = delete;
      file_save() : file_open() {
        _title = _T("Save File");
      }
      bool run(window * owner) { return _run<::GetSaveFileName>(owner); }
    };

  }
}
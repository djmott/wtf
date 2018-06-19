/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {
      TCHAR sSTATUSCLASSNAME[] = STATUSCLASSNAME;
    }

    /** @class statusbar
    Displays status information normally at the bottom of a form
    @ingroup Widgets
    */
    struct statusbar : window_impl<statusbar,
      policy::has_style,
      policy::has_font,
      policy::has_move,
      policy::wm_notify
    > {
      static constexpr DWORD Style = WS_CHILD | WS_VISIBLE | SBARS_TOOLTIPS | SBARS_SIZEGRIP;
      /** @class part
      represents a section of a status bar
      */
      struct part {
        using pointer = std::shared_ptr<part>;

        static constexpr int default_width = 100;

        enum class border_styles {
          none,
          raised,
          lowered,
        };

        constexpr bool spring() const { return -1 == _width; }
        void spring(bool newval) { 
          for (auto & item : _parent->_parts._inner) {
            item->_width = -1;
          }
          _width = -1;
          _parent->set_parts();
        }

        int width() const { return _width; }
        void width(int newval) {
          _width = newval;
          _parent->set_parts();
        }

        border_styles border() const { return _border; }
        void border(border_styles newval) {
          _border = newval;
          _parent->set_parts();
        }

        const tstring& text() const { return _text; }
        void text(const tstring& newval) {
          _text = newval;
          _parent->set_parts();
        }

        /** @class collection
        maintains a collection of parts
        */
        struct collection {
          size_t size() const { return _inner.size(); }

          typename part::pointer add(const tstring& text, int width = part::default_width, part::border_styles border = part::border_styles::lowered) {
            typename part::pointer oRet(new part(_parent, text, width, border));
            _inner.push_back(oRet);
            _parent->set_parts();
            return oRet;
          }

        protected:
          friend struct statusbar;
          friend struct part;
          collection(statusbar * parent) : _parent(parent){}
          statusbar * _parent;
          std::vector<typename part::pointer> _inner;
        };

      protected:
        friend struct statusbar;
        part(statusbar * parent, const tstring& sText, int iWidth, border_styles iBorder) 
          : _parent(parent), _text(sText), _width(iWidth), _border(iBorder) {}
        statusbar * _parent;
        tstring _text;
        int _width;
        border_styles _border;
      };

      bool size_grip() const { return get_style<SBARS_SIZEGRIP>(); }
      void size_grip(bool newval) { return set_style<SBARS_SIZEGRIP>(newval); }

      part::collection& parts() { return _parts; }
      const part::collection& parts() const { return _parts; }

      statusbar() : _parts(this){}

    protected:
      friend struct part;
      friend struct part::collection;
      part::collection _parts;

      void set_parts() {
        std::vector<int> positions;
        int icurrent = 0;
        for (const auto & item : _parts._inner) {
          if (-1 == item->_width) {
            positions.push_back(-1);
          }else {
            icurrent += item->_width; 
            positions.push_back(icurrent);
          }
        }
        wtf::exception::throw_lasterr_if(::SendMessage(static_cast<HWND>(*this), SB_SETPARTS, positions.size(), reinterpret_cast<LPARAM>(&positions[0])), [](LRESULT l) { return FALSE == l; });
        for (uint32_t i = 0; i < _parts._inner.size(); ++i) {
          uint32_t iFlags = 0;
          if (part::border_styles::none == _parts._inner[i]->_border) iFlags = SBT_NOBORDERS;
          if (part::border_styles::raised == _parts._inner[i]->_border) iFlags = SBT_POPOUT;
          wtf::exception::throw_lasterr_if(::SendMessage(*this, SB_SETTEXT, i | iFlags, reinterpret_cast<LPARAM>(&_parts._inner[i]->_text[0])), [](LRESULT l) { return FALSE == l; });
        }
        ::SendMessage(*this, SB_SIMPLE, FALSE, 0);
      }

    };

  }

  template <WNDPROC window_proc> struct window_class<controls::statusbar, window_proc> : super_window_class<controls::_::sSTATUSCLASSNAME, controls::statusbar, window_proc> {};
  
}
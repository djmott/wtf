/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {


    struct font : private LOGFONT {

      using map = std::map<tstring, font>;

      font() = delete;
      ~font() = default;
      font(font&&) = default;
      font& operator=(font&&) = default;
      font& operator=(const font& src) = default;

      font(const font &src) noexcept { memcpy(this, &src, sizeof(LOGFONT)); }

      font(const LOGFONT &src) noexcept { memcpy(this, &src, sizeof(LOGFONT)); }

      enum class weights{
        thin = FW_THIN,
        extra_light = FW_EXTRALIGHT,
        light = FW_LIGHT,
        normal = FW_NORMAL,
        semi_bold = FW_SEMIBOLD,
        bold = FW_BOLD,
        extra_bold = FW_EXTRABOLD,
        heavy = FW_HEAVY,
        black = FW_BLACK,
      };

      weights weight() const noexcept { return static_cast<weights>(lfWeight); }
      void weight(weights newval) noexcept { lfWeight = static_cast<LONG>(newval); }

      bool italic() const noexcept { return (lfItalic ? true : false); }
      void italic(bool newval) noexcept { lfItalic = (newval ? 1 : 0); }

      bool strikeout() const noexcept { return (lfStrikeOut ? true : false); }
      void strikeout(bool newval) noexcept { lfStrikeOut = (newval ? 1 : 0); }

      bool underline() const noexcept { return (lfUnderline ? true : false); }
      void underline(bool newval) noexcept { lfUnderline = (newval ? 1 : 0); }

      LONG height() const noexcept { return lfHeight; }
      void height(LONG newval) noexcept { lfHeight = newval; }

      LONG width() const noexcept { return lfWidth; }
      void width(LONG newval) noexcept { lfWidth = newval; }



      struct handle : std::unique_ptr<HFONT__, void (*)(HFONT)> {
        operator HFONT() const noexcept { return get(); }

        ~handle() = default;
        handle() = delete;

        handle(handle &&src) = default;
        handle &operator=(handle &&) = default;
        handle(const handle&) = delete;
        handle &operator=(const handle &) = delete;

        static handle ansi_fixed()  {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(ANSI_FIXED_FONT), [](HFONT f) noexcept { return !f; }), [](HFONT) noexcept {});
        }

        static handle ansi_variable()  {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(ANSI_VAR_FONT), [](HFONT f) noexcept { return !f; }), [](HFONT) noexcept {});
        }

        static handle device_default()  {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(DEVICE_DEFAULT_FONT), [](HFONT f) noexcept { return !f; }), [](HFONT) noexcept {});
        }

        static handle gui_default()  {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(DEFAULT_GUI_FONT), [](HFONT f) noexcept { return !f; }), [](HFONT) noexcept {});
        }

        static handle oem_fixed()  {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(OEM_FIXED_FONT), [](HFONT f) noexcept { return !f; }), [](HFONT) noexcept {});
        }

        static handle system()  {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(SYSTEM_FONT), [](HFONT f) noexcept { return !f; }), [](HFONT) noexcept {});
        }

        static handle system_fixed()  {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(SYSTEM_FIXED_FONT), [](HFONT f) noexcept { return !f; }), [](HFONT) noexcept {});
        }

      protected:
        friend struct font;

        template<typename ... _ArgTs>
        handle(_ArgTs &&...oArgs) noexcept : unique_ptr(std::forward<_ArgTs>(oArgs)...) {}
      };

      handle open() const  {
        return handle(wtf::exception::throw_lasterr_if(::CreateFontIndirect(this), [](HFONT f) noexcept { return !f; }),
                      [](HFONT f) noexcept { ::DeleteObject(f); });
      }


    protected:
    };
  }


#pragma once

namespace wtf {


    struct font : LOGFONT {


      using map = std::map<tstring, font>;

      font(const font &src) { memcpy(this, &src, sizeof(LOGFONT)); }

      font(const LOGFONT &src) { memcpy(this, &src, sizeof(LOGFONT)); }

      font &operator=(const font &src) {
        if (&src == this) return *this;
        memcpy(this, &src, sizeof(LOGFONT));
        return *this;
      }


      struct handle : std::unique_ptr<HFONT__, void (*)(HFONT)> {
        operator HFONT() const { return get(); }

        handle(handle &&src) : unique_ptr(std::move(src)) {}

        handle &operator=(handle &&src) {
          unique_ptr::swap(src);
          return *this;
        }

        handle(const handle&) = delete;
        handle &operator=(const handle &) = delete;

        static handle ansi_fixed() {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(ANSI_FIXED_FONT), [](HFONT f) { return !f; }), [](HFONT) {});
        }

        static handle ansi_variable() {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(ANSI_VAR_FONT), [](HFONT f) { return !f; }), [](HFONT) {});
        }

        static handle device_default() {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(DEVICE_DEFAULT_FONT), [](HFONT f) { return !f; }), [](HFONT) {});
        }

        static handle gui_default() {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(DEFAULT_GUI_FONT), [](HFONT f) { return !f; }), [](HFONT) {});
        }

        static handle oem_fixed() {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(OEM_FIXED_FONT), [](HFONT f) { return !f; }), [](HFONT) {});
        }

        static handle system() {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(SYSTEM_FONT), [](HFONT f) { return !f; }), [](HFONT) {});
        }

        static handle system_fixed() {
          return handle(wtf::exception::throw_lasterr_if((HFONT)
          ::GetStockObject(SYSTEM_FIXED_FONT), [](HFONT f) { return !f; }), [](HFONT) {});
        }

      protected:
        friend struct font;

        template<typename ... _ArgTs>
        handle(_ArgTs &&...oArgs) : unique_ptr(std::forward<_ArgTs>(oArgs)...) {}
      };

      handle open() const {
        return handle(wtf::exception::throw_lasterr_if(::CreateFontIndirect(this), [](HFONT f) { return !f; }),
                      [](HFONT f) { ::DeleteObject(f); });
      }


    protected:
    };
  }


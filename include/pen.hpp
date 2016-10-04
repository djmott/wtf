#pragma once

namespace wtf {

    struct pen : std::unique_ptr<HPEN__, void (*)(HPEN)> {
      enum class style {
        solid = PS_SOLID,
        dash = PS_DASH,
        dot = PS_DOT,
        dash_dot = PS_DASHDOT,
        dash_dot_dot = PS_DASHDOTDOT,
        invisible = PS_NULL,
        inside_frame = PS_INSIDEFRAME,
      };

      static pen create(style Style, int width, COLORREF color) {
        return pen(
          wtf::exception::throw_lasterr_if(CreatePen(static_cast<int>(Style), width, color), [](HPEN p) { return !p; }),
          [](HPEN p) { ::DeleteObject(p); });
      }

      static pen create(style Style, int width, system_colors color) {
        return pen(wtf::exception::throw_lasterr_if(
          CreatePen(static_cast<int>(Style), width, GetSysColor(static_cast<int>(color))), [](HPEN p) { return !p; }),
                   [](HPEN p) { ::DeleteObject(p); });
      }

      pen(pen &&src) : unique_ptr(std::move(src)) {}

      operator HPEN() const { return get(); }

      pen &operator=(pen &&src) {
        unique_ptr::swap(src);
        return *this;
      }

    protected:
      template<typename ... _ArgTs>
      pen(_ArgTs...oArgs) : unique_ptr(std::forward<_ArgTs>(oArgs)...) {}
    };
  }


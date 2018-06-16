/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

    struct pen : std::shared_ptr<HPEN__> {
      enum class style {
        solid = PS_SOLID,
        dash = PS_DASH,
        dot = PS_DOT,
        dash_dot = PS_DASHDOT,
        dash_dot_dot = PS_DASHDOTDOT,
        invisible = PS_NULL,
        inside_frame = PS_INSIDEFRAME,
      };

      static pen create(style Style, int width, COLORREF color)  {
        return pen(
          wtf::exception::throw_lasterr_if(CreatePen(static_cast<int>(Style), width, color), [](HPEN p) noexcept { return !p; }),
          [](HPEN p) noexcept { ::DeleteObject(p); });
      }

      static pen create(style Style, int width, system_colors color)  {
        return pen(wtf::exception::throw_lasterr_if(
          CreatePen(static_cast<int>(Style), width, GetSysColor(static_cast<int>(color))), [](HPEN p) noexcept { return !p; }),
                   [](HPEN p) noexcept { ::DeleteObject(p); });
      }

      pen() = delete;
      ~pen() = default;

      pen(pen &&src) noexcept : shared_ptr(std::move(src)) {}

      operator HPEN() const noexcept { return get(); }

      pen &operator=(pen &&src) noexcept {
        shared_ptr::swap(src);
        return *this;
      }

      pen(const pen&) = delete;
      pen &operator=(const pen &) = delete;

    protected:
      template<typename ... _ArgTs>
      pen(_ArgTs...oArgs)  : shared_ptr(std::forward<_ArgTs>(oArgs)...) {}
    };
  }


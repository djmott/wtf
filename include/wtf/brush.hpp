/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf {
    struct brush : std::shared_ptr<HBRUSH__> {
      enum class hatch_style {
        b_diagonal = HS_BDIAGONAL,
        cross = HS_CROSS,
        diag_cross = HS_DIAGCROSS,
        f_diagonal = HS_FDIAGONAL,
        horizontal = HS_HORIZONTAL,
        vertical = HS_VERTICAL,
      };

      brush(const brush &) = delete;

      brush &operator=(const brush &) = delete;

      brush(brush &&src) : shared_ptr(std::move(src)) {}

      brush &operator=(brush &&src) {
        shared_ptr::swap(src);
        return *this;
      }

      static brush system_brush(system_colors color) {
        return brush(
          wtf::exception::throw_lasterr_if(GetSysColorBrush(static_cast<int>(color)), [](HBRUSH b) { return !b; }),
          [](HBRUSH) {});
      }

      static brush solid_brush(COLORREF color){
        return brush(wtf::exception::throw_lasterr_if(CreateSolidBrush(color), [](HBRUSH b){ return !b; }),
                     [](HBRUSH b){ ::DeleteObject(b); });
      }

      static brush solid_brush(system_colors color){
        return brush(wtf::exception::throw_lasterr_if(CreateSolidBrush(GetSysColor(static_cast<int>(color))), [](HBRUSH b){ return !b; }),
                     [](HBRUSH b){ ::DeleteObject(b); });
      }

      static brush hatch_brush(hatch_style style, COLORREF color) {
        return brush(wtf::exception::throw_lasterr_if(CreateHatchBrush(static_cast<int>(style), color),
                                                      [](HBRUSH b) { return !b; }),
                     [](HBRUSH b) { ::DeleteObject(b); });
      }

      operator HBRUSH() const { return get(); }

    protected:
      template<typename ... _ArgTs>
      explicit brush(_ArgTs &&...oArgs) : shared_ptr(std::forward<_ArgTs>(oArgs)...) {}
    };

}

/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

    struct region : std::shared_ptr<HRGN__> {

      region() = delete;
      ~region() = default;

      region(region &&src) noexcept : shared_ptr(std::move(src)) {}

      region &operator=(region &&src) noexcept {
        shared_ptr::swap(src);
        return *this;
      }

      region(const region&) = delete;
      region &operator=(const region &) = delete;

      static region attach(HRGN__* newval) noexcept {
        return region(newval, [](HRGN)noexcept {});
      }

      static region create_round_rect(int left, int top, int right, int bottom, int width, int height) {
        return region(wtf::exception::throw_lasterr_if(::CreateRoundRectRgn(left, top, right, bottom, width, height), [](HRGN r)noexcept { return !r; }), [](HRGN h)noexcept { ::DeleteObject(h); });
      }

      static region create_elliptic(int left, int top, int right, int bottom) {
        return region(wtf::exception::throw_lasterr_if(::CreateEllipticRgn(left, top, right, bottom), [](HRGN r)noexcept { return !r; }), [](HRGN h)noexcept { ::DeleteObject(h); });
      }

      operator HRGN() const noexcept { return get(); }
      HRGN operator*() const noexcept { return get(); }

      template <typename _device_contextT>
      void clip(const _device_contextT& dc) {
        wtf::exception::throw_lasterr_if(::SelectClipRgn(dc, *this), [](int i)noexcept {return ERROR == i; });
      }

      void offset(const point<coord_frame::screen>& p) {
        wtf::exception::throw_lasterr_if(::OffsetRgn(*this, -p.x, -p.y), [](int i)noexcept {return ERROR == i; });
      }

    protected:
      template<typename ... _ArgTs>
      region(_ArgTs &&...oArgs)  : shared_ptr(std::forward<_ArgTs>(oArgs)...) {}
    };
  }


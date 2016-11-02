/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

    struct region : std::shared_ptr<HRGN__> {

      region(region &&src) : shared_ptr(std::move(src)) {}

      region &operator=(region &&src) {
        shared_ptr::swap(src);
        return *this;
      }

      region(const region&) = delete;
      region &operator=(const region &) = delete;

      static region attach(HRGN__* newval){
        return region(newval, [](HRGN){});
      }

      static region create_round_rect(int left, int top, int right, int bottom, int width, int height){
        return region(wtf::exception::throw_lasterr_if(::CreateRoundRectRgn(left, top, right, bottom, width, height), [](HRGN r){ return !r; }), [](HRGN h){ ::DeleteObject(h); });
      }

      static region create_elliptic(int left, int top, int right, int bottom){
        return region(wtf::exception::throw_lasterr_if(::CreateEllipticRgn(left, top, right, bottom), [](HRGN r){ return !r; }), [](HRGN h){ ::DeleteObject(h); });
      }

      operator HRGN() const { return get(); }
      HRGN operator*() const{ return get(); }

      template <typename _device_contextT>
      void clip(const _device_contextT& dc){
        wtf::exception::throw_lasterr_if(::SelectClipRgn(dc, *this), [](int i){return ERROR == i; });
      }

      void offset(const point<coord_frame::screen>& p){
        wtf::exception::throw_lasterr_if(::OffsetRgn(*this, -p.x, -p.y), [](int i){return ERROR == i; });
      }

    protected:
      template<typename ... _ArgTs>
      region(_ArgTs &&...oArgs) : shared_ptr(std::forward<_ArgTs>(oArgs)...) {}
    };
  }


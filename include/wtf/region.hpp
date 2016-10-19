#pragma once

namespace wtf {

    struct region : std::unique_ptr<HRGN__, void (*)(HRGN)> {

      region(region &&src) : unique_ptr(std::move(src)) {}

      region &operator=(region &&src) {
        unique_ptr::swap(src);
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

      template <typename _device_contextT>
      void clip(const _device_contextT& dc){
        wtf::exception::throw_lasterr_if(::SelectClipRgn(dc, *this), [](int i){return ERROR == i; });
      }

    protected:
      template<typename ... _ArgTs>
      region(_ArgTs &&...oArgs) : unique_ptr(std::forward<_ArgTs>(oArgs)...) {}
    };
  }


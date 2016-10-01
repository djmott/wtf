#pragma once

namespace wtf{


  struct region : std::unique_ptr<HRGN__, void(*)(HRGN)>{



    static region get_window_region(HWND hWnd){
      region oRet(wtf::exception::throw_lasterr_if(CreateRectRgn(0, 0, 0, 0), [](HRGN h){ return !h; }), [](HRGN h){ DeleteObject(h); });
      return oRet;
    }

    region(region&& src) : unique_ptr(std::move(src)){}

    region& operator=(region&& src){
      unique_ptr::swap(std::move(src));
      return *this;
    }


    operator HRGN() const{ return get(); }

  protected:
    template <typename ... _ArgTs> region(_ArgTs&&...oArgs) : unique_ptr(std::forward<_ArgTs>(oArgs)...){}
    };




}
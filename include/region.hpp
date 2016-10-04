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


      operator HRGN() const { return get(); }

    protected:
      template<typename ... _ArgTs>
      region(_ArgTs &&...oArgs) : unique_ptr(std::forward<_ArgTs>(oArgs)...) {}
    };
  }


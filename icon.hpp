#pragma once

namespace wtf{

  struct icon : std::unique_ptr<HICON__, void (*)(HICON)>{
    enum class style{
      application = (LONG_PTR)IDI_APPLICATION,
      asterisk = (LONG_PTR)IDI_ASTERISK,
      exclamation = (LONG_PTR)IDI_EXCLAMATION,
      hand = (LONG_PTR)IDI_HAND,
      question = (LONG_PTR)IDI_QUESTION,
      winlogo = (LONG_PTR)IDI_WINLOGO,
      shield = (LONG_PTR)IDI_SHIELD,
    };

    static icon system(style Style){
      return icon(wtf::exception::throw_lasterr_if(::LoadIcon(nullptr, reinterpret_cast<LPCTSTR>(Style)), [](HICON h){return !h; }), [](HICON){ });
    }

    icon(icon&& src) : unique_ptr(std::move(src)){}

    icon& operator=(icon&& src){
      unique_ptr::swap(std::move(src));
      return *this;
    }

    operator HICON() const{ return get(); }

  protected:
    template <typename ... _ArgTs> icon(_ArgTs&&...oArgs) : unique_ptr(std::forward<_ArgTs>(oArgs)...){}

  };


}
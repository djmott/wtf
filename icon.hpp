#pragma once

namespace wtf{
  struct icon : std::unique_ptr<HICON__, void(*)(HICON)>{
    enum class style{
      application = 32512,
      asterisk = 32516,
      exclamation = 32515,
      hand = 32513,
      question = 32514,
      winlogo = 32517,
      shield = 32518,
    };

    static icon system(style Style){
      return icon(wtf::exception::throw_lasterr_if(::LoadIcon(nullptr, MAKEINTRESOURCE(Style)), [](HICON h){return !h; }), [](HICON){});
    }

    icon(icon&& src) : unique_ptr(std::move(src)){}

    icon& operator=(icon&& src){
      unique_ptr::swap(src);
      return *this;
    }

    operator HICON() const{ return get(); }

  protected:
    template <typename ... _ArgTs> icon(_ArgTs&&...oArgs) : unique_ptr(std::forward<_ArgTs>(oArgs)...){}
  };
}
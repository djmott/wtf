#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT> struct has_zorder : _SuperT{
      enum class position{
        bottom = 1,
        no_top_most = -2,
        top = 0,
        top_most = -1,
      };

      void zorder(position pos){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, static_cast<HWND>(pos), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE), [](BOOL b){ return !b; });
      }
      void zorder(HWND insert_after){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, insert_after, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE), [](BOOL b){ return !b; });
      }

    };
  }
}
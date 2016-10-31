#pragma once

namespace wtf{
  enum class zorders{
    bottom = 1,
    no_top_most = -2,
    top = 0,
    top_most = -1,
  };

  namespace policy{
    template <typename _SuperT>
    class has_zorder : public _SuperT{

      
    public:

      void zorder(zorders pos){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, reinterpret_cast<HWND>(pos), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS), [](BOOL b){ return !b; });
      }
      void zorder(HWND insert_after){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, insert_after, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS), [](BOOL b){ return !b; });
      }
    protected:
      explicit has_zorder(iwindow * pParent) : _SuperT(pParent){}

    };
  }
}

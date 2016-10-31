#pragma once

namespace wtf{
  /** has_timer
  * Adds timer creation and produces timer events
  */
  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class has_timer : public _SuperT{
      
    public:

      UINT_PTR set_timer(UINT elapse){
        _next_timer_id++;
        return wtf::exception::throw_lasterr_if(::SetTimer(*this, _next_timer_id, elapse, nullptr),
                                                [](UINT_PTR x){ return !x; });
      }

      void set_timer(UINT elapse, UINT_PTR timer_id){
        wtf::exception::throw_lasterr_if(::SetTimer(*this, timer_id, elapse, nullptr),
                                         [](UINT_PTR x){ return !x; });
      }

      void kill_timer(UINT_PTR timer_id){
        wtf::exception::throw_lasterr_if(::KillTimer(*this, timer_id), [](BOOL x){ return !x; });
      }

    protected:

      explicit has_timer(iwindow * pParent) : _SuperT(pParent){}
    private:
      UINT_PTR _next_timer_id = 1;
    };
  }
}
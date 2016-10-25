#pragma once

namespace wtf {
    /** has_timer
    * Adds timer creation and produces timer events
    */
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_timer, _Policies...> : public window<_ImplT, _Policies...>{
      using __super_t = window<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window;
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

      void kill_timer(UINT_PTR timer_id) {
        wtf::exception::throw_lasterr_if(::KillTimer(*this, timer_id), [](BOOL x) { return !x; });
      }


    protected:

      explicit window(iwindow * pParent) : __super_t(pParent){}
    private:
      UINT_PTR _next_timer_id = 1;
    };

  }

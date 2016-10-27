#pragma once

namespace wtf {

    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_move, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:

      enum class wm_size_flags {
        hide = SIZE_MAXHIDE,
        maximized = SIZE_MAXIMIZED,
        show = SIZE_MAXSHOW,
        minimized = SIZE_MINIMIZED,
        restored = SIZE_RESTORED,
      };

      void move(int x, int y, int width, int height, bool repaint = true) {
        wtf::exception::throw_lasterr_if(::MoveWindow(*this, x, y, width, height, repaint ? TRUE : FALSE),
                                         [](BOOL b) { return !b; });
      }

      virtual int left() const{
        auto rc = rect<coord_frame::client>::get(*this);
        MapWindowPoints(*this, *parent(), reinterpret_cast<LPPOINT>(&rc), 2);
        return rc.left;
      }

      virtual int top() const {
        auto rc = rect<coord_frame::client>::get(*this);
        SetLastError(ERROR_INVALID_PARAMETER);
        MapWindowPoints( *this, *parent(), reinterpret_cast<LPPOINT>(&rc), 2);
        return rc.top; 
      }


      virtual void left(int newval){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, newval, top(), 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b){ return !b; });
      }

      virtual void top(int newval){
        wtf::exception::throw_lasterr_if(::SetWindowPos(*this, 0, left(), newval, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREPOSITION),
                                         [](BOOL b){ return !b; });
      }

    protected:

      explicit window(iwindow * pParent) : __super_t(pParent){}


    };

  }

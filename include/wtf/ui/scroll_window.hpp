#pragma once
namespace wtf{

  namespace policy{


    template <typename _SuperT>
    struct isa_vscroll_window : _SuperT{

      callback<void(int prev_val)> OnVScroll;

    protected:

      explicit isa_vscroll_window(window * pParent) : _SuperT(pParent), _inner(this){}

      virtual void on_vscroll(int prevval){ OnVScroll(prevval); }

    private:

      static const int scrollbar_size = 20;

      LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS * pSizes) override{
        auto lRet = _SuperT::on_wm_nccalcsize(pSizes);
        pSizes->rgrc[0].right -= scrollbar_size;
        return lRet;
      }


      void on_wm_size(const point<coord_frame::client>& p) override{
        auto border = _SuperT::border_width();
        _inner.move(p.x - border - scrollbar_size, border, scrollbar_size, p.y - (border * 2));
        _SuperT::on_wm_size(p);
      }


      struct inner : scroll_bar{

        inner(isa_vscroll_window * pParent) : scroll_bar(pParent), parent(pParent){
          orientation(orientations::vertical);
        }

        void on_value_changed(int prev_val) override{
          parent->on_vscroll(prev_val);
        }
        
        isa_vscroll_window * parent;
      }_inner;

    };


    template <typename _SuperT>
    struct isa_hscroll_window : _SuperT{

      callback<void(int prev_val)> OnHScroll;

    protected:

      explicit isa_hscroll_window(window * pParent) : _SuperT(pParent), _inner(this){}

      virtual void on_hscroll(int prevval){ OnHScroll(prevval); }

    private:

      static const int scrollbar_size = 22;

      LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS * pSizes) override{
        auto lRet = _SuperT::on_wm_nccalcsize(pSizes);
        pSizes->rgrc[0].bottom -= scrollbar_size;
        return lRet;
      }


      void on_wm_size(const point<coord_frame::client>& p) override{
        auto border = _SuperT::border_width();
        _inner.move(border, p.y - border - scrollbar_size, p.x - (border * 2), scrollbar_size);
        _SuperT::on_wm_size(p);
      }

    
      struct inner : scroll_bar{

        inner(isa_hscroll_window * pParent) : scroll_bar(pParent), parent(pParent){
          orientation(orientations::vertical);
        }

        void on_value_changed(int prev_val) override{
          parent->on_hscroll(prev_val);
        }

        isa_hscroll_window * parent;
      }_inner;

    };



    template <typename _SuperT>
    struct isa_scroll_window : _SuperT{

    protected:

      explicit isa_scroll_window(window * pParent) : _SuperT(pParent){}

    };

  }


  namespace _{


    template <> struct policy_traits<policy::isa_vscroll_window>{
      using requires = policy_list<policy::has_border, policy::wm_size>;
    };


    template <> struct policy_traits<policy::isa_hscroll_window>{
      using requires = policy_list<policy::has_border, policy::wm_size>;
    };

    template <> struct policy_traits<policy::isa_scroll_window>{
      using requires = policy_list<policy::isa_vscroll_window, policy::isa_hscroll_window>;
    };

  }

}
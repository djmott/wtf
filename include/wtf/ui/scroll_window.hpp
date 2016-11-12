
#pragma once
namespace wtf{

  namespace policy{


    template <typename _SuperT>
    struct isa_vscroll_window : _SuperT{

      virtual scroll_bar& vscroll_bar(){ return _inner; }
      virtual const scroll_bar& vscroll_bar() const { return _inner; }

    protected:

      explicit isa_vscroll_window(window * pParent) : _SuperT(pParent), _inner(this){}

      static const int scrollbar_size = 20;

      LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS * pSizes) override{
        auto lRet = _SuperT::on_wm_nccalcsize(pSizes);
        if (_inner.visible()) pSizes->rgrc[0].right -= scrollbar_size;
        return lRet;
      }


      void on_wm_size(const point<coord_frame::client>& p) override{
        auto border = _SuperT::border_width();
        _inner.move(p.x, border, p.x + scrollbar_size, p.y - (border * 2));
        _SuperT::on_wm_size(p);
      }

    private:

      struct inner : scroll_bar{

        inner(isa_vscroll_window * pParent) : scroll_bar(pParent), parent(pParent){
          orientation(orientations::vertical);
        }
       
        isa_vscroll_window * parent;
      }_inner;

    };


    template <typename _SuperT>
    struct isa_hscroll_window : _SuperT{

      virtual scroll_bar& hscroll_bar(){ return _inner; }
      virtual const scroll_bar& hscroll_bar() const{ return _inner; }

    protected:

      explicit isa_hscroll_window(window * pParent) : _SuperT(pParent), _inner(this){}

      static const int scrollbar_size = 22;

      LRESULT on_wm_nccalcsize(NCCALCSIZE_PARAMS * pSizes) override{
        auto lRet = _SuperT::on_wm_nccalcsize(pSizes);
        if (_inner.visible()) pSizes->rgrc[0].bottom -= scrollbar_size;
        return lRet;
      }


      void on_wm_size(const point<coord_frame::client>& p) override{
        auto border = _SuperT::border_width();
        _inner.move(border, p.y, p.x - (border * 2), p.y + scrollbar_size);
        _SuperT::on_wm_size(p);
      }

    private:
      struct inner : scroll_bar{

        inner(isa_hscroll_window * pParent) : scroll_bar(pParent), parent(pParent){
          orientation(orientations::vertical);
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
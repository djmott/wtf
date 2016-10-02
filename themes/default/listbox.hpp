#pragma once
namespace wtf{
  namespace default_theme{

      struct listbox : wtf::window<listbox, has_background, has_border, has_click_event, has_paint_event, has_font, has_size>{

        listbox(HWND hParent) : window(hParent), _Panel(*this){
          _Panel.move(0, 0, this->width(), 0);
        }
        void add_item(const tstring& newval){
          std::shared_ptr<label> oItem(new label(*_Panel));
          oItem->text(newval);
          if (!_Items.size()){
            oItem->move(0, 0, oItem->text_size().cx, oItem->text_size().cy);
          } else{
            auto& oLast = _Items.back();
            oItem->move(0, oLast->top() + oLast->height(), oItem->text_size().cx, oItem->text_size().cy);
          }
          _Items.push_back(oItem);
          auto oMaxMetrics = get_max_text_metrics();
          _Panel.move(0,0, oMaxMetrics.cx, oItem->top() + oItem->height());
        }

        

      protected:
        size get_max_text_metrics() const{
          size oRet;
          for (const auto & oItem : _Items){
            auto oSize = oItem->text_size();
            if (oRet.cx < oSize.cx) oRet.cx = oSize.cx;
            if (oRet.cy < oSize.cy) oRet.cy = oSize.cy;
          }
          return oRet;
        }

        virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled) override{
          if (WM_PAINT == umsg){

          }
          return 0;
        }

        panel _Panel;
        std::vector<std::shared_ptr<label>> _Items;

      };
    }

}
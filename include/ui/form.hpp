#pragma once

namespace wtf{

    struct form : window<form, policy::has_icon, policy::has_show, policy::has_cursor,
      policy::has_titlebar, policy::has_size, policy::has_close, policy::has_paint,
      policy::has_mouse, policy::has_click>
    {
      static const DWORD ExStyle = WS_EX_OVERLAPPEDWINDOW | WS_EX_CONTROLPARENT;
      static const DWORD Style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;

      form(HWND hParent) : window(hParent, true){}
      form() : form(nullptr){}
      virtual ~form() = default;

      int exec(bool show_window=true){
        _QuitOnDestroy = true;
        if (show_window) show();
        try{
          message oMsg;
          while (oMsg.get()){
            oMsg.translate();
            oMsg.dispatch();
          }
          return 0;
        }
        catch (const wtf::exception& ex){
          std::string sTemp = ex.what();
          sTemp += "\n\nat:\n\n";
          sTemp += ex.file();
          sTemp += "(";
          sTemp += std::to_string(ex.line());
          sTemp += ")\n\n";
          sTemp += ex.code();
          tstring sMsg;
          std::copy(sTemp.begin(), sTemp.end(), std::back_inserter(sMsg));

          auto iRet = message_box::exec(nullptr, sMsg, _T("An exception occurred"), message_box::buttons::abort_retry_ignore, message_box::icons::stop);
          if (message_box::response::abort == iRet) abort();
          if (message_box::response::ignore == iRet) return -1;
          throw;
        }
      }

    protected:


      bool _QuitOnDestroy = false;

      virtual LRESULT handle_message(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam, bool& bhandled){
        if (WM_DESTROY == umsg && _QuitOnDestroy){
          PostQuitMessage(0);
          bhandled = true;
          return 0;
        }
        std::cout << "Form got a " << _::msg_name(umsg) << std::endl;
        return 0;
      }


    };

}

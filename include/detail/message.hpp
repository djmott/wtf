#pragma once

namespace wtf{
  struct message : MSG {
    bool get(HWND hWnd = 0, UINT msgmin = 0, UINT msgmax = 0) {
      return (exception::throw_lasterr_if(
        ::GetMessage(this, hWnd, msgmin, msgmax), [](BOOL b) { return -1 == b; }
      ) ? true : false);
    }

    void translate() const { ::TranslateMessage(this); }

    void dispatch() const { ::DispatchMessage(this); }

    bool peek(HWND hWnd = 0, UINT msgmin = 0, UINT msgmax = 0, UINT remove = PM_NOREMOVE) {
      return (exception::throw_lasterr_if(
        ::PeekMessage(this, hWnd, msgmin, msgmax, remove), [](BOOL b) { return FALSE == b; }
      ) ? true : false);
    }

    int pump() {
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
  };
}

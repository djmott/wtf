#pragma once

namespace wtf{
  namespace _{
    struct non_client_metrics : NONCLIENTMETRICS{
      non_client_metrics(){
        memset(this, 0, sizeof(NONCLIENTMETRICS));
        cbSize = sizeof(NONCLIENTMETRICS);
      }

      static non_client_metrics get(){
        non_client_metrics oRet;
        wtf::exception::throw_lasterr_if(
          SystemParametersInfo(SPI_GETNONCLIENTMETRICS, oRet.cbSize, &oRet, 0),
          [](BOOL b){ return !b; }
        );
        return oRet;
      }
    };
  }

}
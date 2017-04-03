#ifndef PEER_H
#define PEER_H

#include <QObject>
#include <QWidget>
#include <globalheader.h>

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <iostream>
#include <chrono>
 #define BOOST_LOG_DYN_LINK
//#include "common.h"
#include "core/common.h"

#if defined __IMS__
#include "core/peer_ims.h"
#endif   /* __IMS__ */

#if defined __DBS__ || defined __ACS__
#if defined __monitor__
#include "core/monitor_dbs.h"    // Includes peer_dbs.h
#else
#include "core/peer_dbs.h"
#endif   /* __monitor__ */
#endif   /* __DBS__ || __ACS__ */

#if defined __LRS__
#if defined __monitor__
#include "core/monitor_lrs.h"   // Includes monitor_dbs.h
#else
#include "core/peer_dbs.h"
#endif   /* __monitor__ */
#endif   /* __LRS__ */

#if defined __NTS__
#if defined __monitor__
#include "core/monitor_nts.h"   // Includes Peer_NTS
#else
//#include "core/peer_nts.h"      // Includes peer_dbs.h
#include "core/peer_symsp.h"    // Includes peer_nts.h, which includes peer_dbs.h
#endif    /* __monitor__ */
#endif    /* __NTS__ */

#if defined __EMS__
#if defined __monitor__
#include "core/monitor_ems.h"
#else
#include "core/peer_ems.h"
#endif   /* __monitor__ */
#endif   /* __EMS__ */

#include "util/trace.h"

using namespace std;
using namespace boost;

//struct Source {
//  ip::address addr;
//  PORT_TYPE port;
//}source;

namespace p2psp{


    class peer : public QWidget,
        #if defined __IMS__
            public Peer_IMS
        #endif

        #if defined __DBS__ || defined __ACS__
        #if defined __monitor__
            public Monitor_DBS
        #else
            public Peer_DBS
        #endif /* __monitor__ */
        #endif /* __DBS__ || __ACS__ */

        #if defined __LRS__
        #if defined __monitor__
            public Monitor_LRS
        #else
            public Peer_DBS
        #endif
        #endif /* __LRS__ */

        #if defined __NTS__
        #if defined __monitor__
            public Monitor_NTS
        #else
            public Peer_SYMSP
        #endif /* __monitor__ */
        #endif /* __NTS__ */


        #if defined __EMS__
        #if defined __monitor__
            public Monitor_EMS
        #else
            public Peer_EMS
        #endif
        #endif /* __EMS__ */

    {

           Q_OBJECT
        public:

        explicit peer(QWidget *parent = 0);

        void ReceiveSourceEndpoint();
        void ReceiveChannel();
        std::string GetChannel();
        void ReceiveHeaderSize();
        void ConnectToTheSource();
        void RequestHeader();
        void RelayHeader();
        void WaitForThePlayer();
        bool PlayChunk(int chunk);
        void SetPlayerPort(uint16_t player_port);
        uint16_t GetPlayerPort();

        uint16_t GetDefaultPlayerPort();
        void SetGETMessage();
        int GetHeaderSize();

        ip::address GetSourceAddr();
        unsigned short GetSourcePort();

    protected:

      PORT_TYPE player_port_;
      io_service io_service_;
      ip::tcp::acceptor acceptor_;
      ip::tcp::socket source_socket_;
      ip::tcp::socket player_socket_;
      HEADER_SIZE_TYPE header_size_;
    //  struct Source source;
      std::string GET_message_;
      std::string channel_;
      struct Source{
      ip::address addr;
      PORT_TYPE port;
      }source;
    signals:

    public slots:
    };
}

#endif // PEER_H

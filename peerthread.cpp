#include "peerthread.h"

PeerThread::PeerThread(QObject *parent) : QThread(parent)
{
    peerobj = new p2psp::peer();
    player_port = peerobj->GetDefaultPlayerPort();
    splitter_addr = p2psp::Peer_core::GetDefaultSplitterAddr().to_string();
    splitter_port = p2psp::Peer_core::GetDefaultSplitterPort();
#ifndef __IMS__
    max_chunk_debt = p2psp::Peer_DBS::GetDefaultMaxChunkDebt();
    team_port = p2psp::Peer_core::GetDefaultTeamPort();
#endif
#if defined __NTS__ && not defined __monitor__
    source_port_step = 0;
#endif

}

void PeerThread::Play(runDialogStorage runParameters)
{
    //Later can be changed as class constructor in runDialogStorage
    runtimeParameters.setPlayerPort(runParameters.getPlayerPort());
    runtimeParameters.setSplitterPort(runParameters.getSplitterPort());
    runtimeParameters.setSplitterAddress(runParameters.getSplitterAddress());
#ifndef __IMS__
    runtimeParameters.setMaxChunk(runParameters.getMaxChunk());
    runtimeParameters.setLocalHost(runParameters.getLocalHost());
    runtimeParameters.setTeamPort(runParameters.getTeamPort());
#endif
#ifdef __NTS__
#ifndef __monitor__
    runtimeParameters.setSourcePortStep(runParameters.getSourcePortStep());
#endif
#endif

    if (!isRunning())
    {
        if (isStopped())
        {
            stop1 = false;
        }
        start(LowPriority);
    }
}
void PeerThread::run()
{
    usingHeaders();
    setParameters();
    peerInitialization();
    calcBufferingTime();
    peerobj->Start();
    setLocalVariables();
    while(peerobj->IsPlayerAlive())
    {
         boost::this_thread::sleep(boost::posix_time::seconds(1));
        // delay(1000);
        process();
        printCurrentTime();
    }
}
void PeerThread::usingHeaders()
{
#if defined __IMS__
    std::cout << "Using Peer_IMS" << std::endl;
#endif

#if defined __DBS__ || defined __ACS__
#if defined __monitor__
    std::cout << "Using Monitor_DBS" << std::endl;
#else
    std::cout << "Using Peer_DBS" << std::endl;
#endif /* __monitor__ */
#endif /* __DBS__ || __ACS__ */

#if defined __LRS__
#if defined __monitor__
    std::cout << "Using Monitor_LRS" << std::endl;
#else
    std::cout << "Using Peer_DBS" << std::endl;
#endif
#endif /* __LRS__ */

#if defined __NTS__
#if defined __monitor__
    std::cout << "Using Monitor_NTS" << std::endl;
#else
    std::cout << "Using Peer_NTS" << std::endl;
#endif /* __monitor__ */
#endif /* __NTS__ */

#if defined __EMS__
#if defined __monitor__
    std::cout << "Using Monitor_EMS" << std::endl;
#else
    std::cout << "Using Peer_EMS" << std::endl;
#endif /* __monitor__ */
#endif /* __EMS__ */
}


void PeerThread::setParameters()
{
//    cout<<"Default Player Port"<<player_port<<endl;
//    cout<<"Player port from UI "<<runtimeParameters.getPlayerPort();
    if(runtimeParameters.getPlayerPort()!=0)
    {
        player_port=runtimeParameters.getPlayerPort();
    }
    peerobj->SetPlayerPort(player_port);
    peerobj->WaitForThePlayer();
    if(runtimeParameters.getSplitterAddress()!="")
    {
        splitter_addr=runtimeParameters.getSplitterAddress();
        peerobj->SetSplitterAddr(ip::address::from_string(splitter_addr));
    }
    if(runtimeParameters.getSplitterPort()!=0)
    {
        splitter_port=runtimeParameters.getSplitterPort();
        peerobj->SetSplitterPort(splitter_port);
    }
    peerobj->ConnectToTheSplitter();
    peerobj->ReceiveSourceEndpoint();
    peerobj->ConnectToTheSource();
    peerobj->ReceiveChannel();
    peerobj->ReceiveHeaderSize();
    peerobj->RequestHeader();
    peerobj->RelayHeader();
    peerobj->ReceiveChunkSize();
    peerobj->ReceiveBufferSize();
#ifdef __IMS__
    peerobj->ReceiveMcastGroup();
#else
    if(runtimeParameters.getMaxChunk()!=0)
    {
        max_chunk_debt=runtimeParameters.getMaxChunk();
        peerobj->SetMaxChunkDebt(max_chunk_debt);
    }
    if(runtimeParameters.getTeamPort()!=0)
    {
        team_port=runtimeParameters.getTeamPort();
        peerobj->SetTeamPort(team_port);
    }
    if(runtimeParameters.getLocalHost())
    {
        peerobj->SetUseLocalHost(true);
    }
#endif
#ifdef __NTS__
#ifndef __monitor__
    if(runtimeParameters.getSourcePortStep()!=0)
    {
        sourcePortStep=runtimeParameters.getSourcePortStep();
        peerobj->SetPortStep(sourcePortStep);
    }
#endif
#endif
}

void PeerThread::peerInitialization()
{
    peerobj->Init();
    peerobj->ListenToTheTeam();

#ifndef __IMS__
    peerobj->ReceiveTheListOfPeers();
//    TRACE("List of peers received");
//    TRACE("Number of peers in the team (excluding me) = "
//          << std::to_string(peerobj->GetNumberOfPeers()));
#endif

    peerobj->SendReadyForReceivingChunks();
    peerobj->DisconnectFromTheSplitter();
}

void PeerThread::calcBufferingTime()
{
    std::cout<< "Buffering ... "<< std::endl << std::flush;
    //time_t start_time = time(NULL);
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //auto start = std::chrono::steady_clock::now();
    peerobj->BufferData();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    /*auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::steady_clock::now() - start);*/
    std::cout << "done" << std::endl;
    std::cout<< "Buffering time = "
             << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0
             << " seconds" << std::endl;
}


void PeerThread::setLocalVariables()
{
    kbps_recvfrom = 0;
    kbps_sendto = 0;
    last_sendto_counter = -1;
    last_recvfrom_counter = peerobj->GetRecvfromCounter();

#ifndef __IMS__

    last_chunk_number = peerobj->GetPlayedChunk();
    kbps_expected_recv = 0;
    if (peerobj->GetSendtoCounter() < 0) {
        last_sendto_counter = 0;
    } else {
        //peer->SetSendtoCounter(0);
        last_sendto_counter = 0;
    }
    team_ratio = 0.0f;
    kbps_expected_sent = 0;
    counter = 0;

#endif
}


void PeerThread::process()
{
    kbps_sendto = int(((peerobj->GetSendtoCounter() - last_sendto_counter) *
                       peerobj->GetChunkSize() * 8) / 1000.0f);
    last_sendto_counter = peerobj->GetSendtoCounter();
    kbps_recvfrom = int(((peerobj->GetRecvfromCounter() - last_recvfrom_counter) *
                         peerobj->GetChunkSize() * 8) / 1000.0f);
    last_recvfrom_counter = peerobj->GetRecvfromCounter();

#ifndef __IMS__

    kbps_expected_recv = int(((peerobj->GetPlayedChunk() - last_chunk_number) *
                              peerobj->GetChunkSize() * 8) / 1000.0f);
    last_chunk_number = peerobj->GetPlayedChunk();
    {
        team_ratio = peerobj->GetPeerList()->size() / (peerobj->GetPeerList()->size() + 1.0f);
    }
    kbps_expected_sent = (int)(kbps_expected_recv * team_ratio);

    if (kbps_expected_recv < kbps_recvfrom) {
//        cout << _SET_COLOR(_GREEN);
    } else if (kbps_expected_recv > kbps_recvfrom) {
//        cout << _SET_COLOR(_RED);
    }

#endif /* not defined __IMS__ */
//        std::cout<< kbps_recvfrom<< _RESET_COLOR();
#ifndef __IMS__
//        std::cout<<kbps_expected_recv;

        if (kbps_expected_sent < kbps_sendto) {
//            std::cout << _SET_COLOR(_GREEN);
        } else if (kbps_expected_sent > kbps_sendto) {
//            std::cout << _SET_COLOR(_RED);
        }

#endif /* not defined __IMS__ */
//        std::cout<< kbps_sendto<< _RESET_COLOR();

#ifndef __IMS__
//        std::cout<< kbps_expected_sent;
#endif /* not defined __IMS__ */

/*Used for Displaying PeerList with ports*/

//#ifndef __IMS__
//    counter = 0;
//    for (std::vector<boost::asio::ip::udp::endpoint>::iterator p = peer->GetPeerList()->begin();
//            p != peer->GetPeerList()->end();
//            ++p) {
//        if (counter < 5) {
//            std::cout << p->address().to_string()
//                      << ","
//                      << std::to_string(p->port())
//                      << " ";
//            counter++;
//        } else {
//            break;
//            std::cout << "";
//        }
//    }
//#endif
//    cout<<"Download"<<kbps_recvfrom<<endl;
//    cout<<"Upload"<<kbps_sendto<<endl;
#ifndef __IMS__
    emit stats(kbps_recvfrom,kbps_sendto,peerobj->GetNumberOfPeers());
#else
    emit stats(kbps_recvfrom,kbps_sendto,0);
#endif
}

void PeerThread::printCurrentTime()
{
//    using boost::posix_time::ptime;
//    using boost::posix_time::second_clock;
//    using boost::posix_time::to_simple_string;
//    using boost::gregorian::day_clock;
//    ptime todayUtc(day_clock::universal_day(), second_clock::universal_time().time_of_day());
//    std::cout << to_simple_string(todayUtc);
}


void PeerThread::delay(int msecs)
{
    QTime dieTime= QTime::currentTime().addMSecs(msecs);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

PeerThread::~PeerThread()
{
    mutex.lock();
    stop1 = true;
    //delete pBgs;
    condition.wakeOne();
    mutex.unlock();
    wait();
}
void PeerThread::msleep(int ms)
{
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}
void PeerThread::Stop()
{
    stop1 = true;
}
bool PeerThread::isStopped() const
{
    return this->stop1;
}


//int run(int argc, const char *argv[]) throw(boost::system::system_error)
//{


//// Argument Parsing

//    const char description[80] = "This is a peer node of a P2PSP team.\n"
//                                 "Parameters";

//    boost::program_options::options_description desc(description);

//    {



//        // TODO: strpe option should expect a list of arguments, not bool
//        desc.add_options()
//        ("help,h", "Produce this help message and exits.")
//#if not defined __IMS__
//        ("max_chunk_debt", boost::program_options::value<int>()->default_value(max_chunk_debt), "Maximum number of times that other peer can not send a chunk to this peer.")
//#endif
//        ("player_port", boost::program_options::value<uint16_t>()->default_value(player_port), "Port to communicate with the player.")
//#if defined __NTS__ && not defined __monitor__
//        ("source_port_step", boost::program_options::value<int>()->default_value(source_port_step), "Source port step forced when behind a sequentially port allocating NAT (conflicts with --chunk_loss_period).")
//#endif
//        ("splitter_addr", boost::program_options::value<std::string>()->default_value(splitter_addr), "IP address or hostname of the splitter.")
//        ("splitter_port", boost::program_options::value<uint16_t>()->default_value(splitter_port), "Listening port of the splitter.")
//#if not defined __IMS__
//        ("team_port", boost::program_options::value<uint16_t>()->default_value(team_port), "Port to communicate with the peers. By default the OS will chose it.")
//        ("use_localhost", "Forces the peer to use localhost instead of the IP of the adapter to connect to the splitter." "Notice that in this case, peers that run outside of the host will not be able to communicate with this peer.")
//#endif
//        ;

//    }

//    boost::program_options::variables_map vm;

//    try {
//        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
//    } catch (std::exception &e) {
//        // If the argument passed is unknown, print the list of available arguments
//        std::cout << desc << "\n";
//        return 1;
//    }

//    boost::program_options::notify(vm);

//    if (vm.count("help")) {
//        std::cout << desc << "\n";
//        return 1;
//    }





// Peer instantiation

//    class Console *peer = new Console();



//    if (vm.count("player_port")) {


//        peer->SetPlayerPort(vm["player_port"].as<uint16_t>());
//        TRACE("Player port = "
//              << peer->GetPlayerPort());


//    }

//    peer->WaitForThePlayer();
//    std::cout
//            << "Player connected"
//            << std::endl;

//    if (vm.count("splitter_addr")) {


//        peer->SetSplitterAddr(ip::address::from_string(vm["splitter_addr"].as<std::string>()));
//        TRACE("Splitter address = "
//              << peer->GetSplitterAddr());


//    }

//    if (vm.count("splitter_port")) {


//        peer->SetSplitterPort(vm["splitter_port"].as<uint16_t>());
//        TRACE("Splitter port = "
//              << peer->GetSplitterPort());


//    }

//    peer->ConnectToTheSplitter();
//    TRACE("Connected to the splitter");
//    /*std::cout
//      << "Real splitter port = "
//      << peer->GetRealSplitterPort()
//      << std::endl;*/

//    peer->ReceiveSourceEndpoint();
//    TRACE("Source = ("
//          << peer->GetSourceAddr()
//          << ","
//          << std::to_string(peer->GetSourcePort())
//          << ")");

//    peer->ConnectToTheSource();
//    TRACE("Connected to the source");

//    peer->ReceiveChannel();
//    TRACE("channel = "
//          << peer->GetChannel());

//    peer->ReceiveHeaderSize();
//    TRACE("Header size = "
//          << peer->GetHeaderSize());

//    peer->RequestHeader();
//    TRACE("Header requested");

//    std::cout << "Relaying the header from the source to the player ... " << std::flush;
//    peer->RelayHeader();
//    std::cout << "done" << std::endl;

//    peer->ReceiveChunkSize();
//    TRACE("Chunk size = "
//          << peer->GetChunkSize());

//    peer->ReceiveBufferSize();
//    TRACE("Buffer size = "
//          << peer->GetBufferSize());

//#if defined __IMS__


//    peer->ReceiveMcastGroup();
//    TRACE("Using IP multicast group = ("
//          << peer->GetMcastAddr().to_string()
//          << ","
//          << peer->GetMcastPort()
//          << ")");



//#else /* __IMS__ */



//    if (vm.count("max_chunk_debt")) {


//        peer->SetMaxChunkDebt(vm["max_chunk_debt"].as<int>());
//        TRACE("Maximum chunk debt = "
//              << peer->GetMaxChunkDebt());


//    }

//    if (vm.count("team_port")) {


//        peer->SetTeamPort(vm["team_port"].as<uint16_t>());
//        TRACE("team_port = "
//              << peer->GetTeamPort());


//    }

//    if (vm.count("use_localhost")) {


//        peer->SetUseLocalHost(true);
//        TRACE("use_localhost = "
//              << peer->GetUseLocalHost());


//    }


//#endif /* __IMS__*/

//#if defined __NTS__
//# if not defined __monitor__


//    if (vm.count("source_port_step")) {
//        peer->SetPortStep(vm["source_port_step"].as<int>());
//    }
//    TRACE("Source port step = "
//          << peer->GetPortStep());


//#endif
//#endif

//    peer->Init();
//    peer->ListenToTheTeam();
//    TRACE("Listening to the team");

//#if not defined __IMS__


//    TRACE("Receiving the list of peers ... ");
//    peer->ReceiveTheListOfPeers();
//    std::cout << "done" << std::endl;
//    TRACE("List of peers received");
//    TRACE("Number of peers in the team (excluding me) = "
//          << std::to_string(peer->GetNumberOfPeers()));


//#endif

//    peer->SendReadyForReceivingChunks();

//    peer->DisconnectFromTheSplitter();
//    TRACE("Recived the configuration from the splitter.");
//    TRACE("Clossing the connection");

//    std::cout
//            << "Buffering ... "
//            << std::endl << std::flush;
//    {
//        //time_t start_time = time(NULL);
//        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//        //auto start = std::chrono::steady_clock::now();
//        peer->BufferData();
//        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//        /*auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
//        (std::chrono::steady_clock::now() - start);*/
//        std::cout << "done" << std::endl;
//        std::cout
//                << "Buffering time = "
//                << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0
//                << " seconds" << std::endl;
//    }
//    peer->Start();
//    //LOG("Peer running in a thread");

//    std::cout << _RESET_COLOR();

//#if defined __IMS__

//    std::cout << "                     | Received |     Sent |" << std::endl;
//    std::cout << "                Time |   (kbps) |   (kbps) |" << std::endl;
//    std::cout << "---------------------+----------+----------+" << std::endl;

//#else

//    std::cout << std::endl;
//    std::cout << "                     | Received Expected |     Sent Expected | Team |" << std::endl;
//    std::cout << "                Time |   (kbps)   (kbps) |   (kbps)   (kbps) | size | Peer list" << std::endl;
//    std::cout << "---------------------+-------------------+-------------------+------+----------..." << std::endl;

//#endif

//    int kbps_recvfrom = 0;
//    int kbps_sendto = 0;
//    int last_sendto_counter = -1;
//    int last_recvfrom_counter = peer->GetRecvfromCounter();

//#if not defined __IMS__

//    int last_chunk_number = peer->GetPlayedChunk();
//    int kbps_expected_recv = 0;
//    if (peer->GetSendtoCounter() < 0) {
//        last_sendto_counter = 0;
//    } else {
//        //peer->SetSendtoCounter(0);
//        last_sendto_counter = 0;
//    }
//    float team_ratio = 0.0f;
//    int kbps_expected_sent = 0;
//    int counter = 0;

//#endif

//    while (peer->IsPlayerAlive()) {
//        boost::this_thread::sleep(boost::posix_time::seconds(1));

//        { /* Print current time */
//            using boost::posix_time::ptime;
//            using boost::posix_time::second_clock;
//            using boost::posix_time::to_simple_string;
//            using boost::gregorian::day_clock;
//            ptime todayUtc(day_clock::universal_day(), second_clock::universal_time().time_of_day());
//            std::cout << to_simple_string(todayUtc);
//        }

//        std::cout
//                << " |";

//        kbps_sendto = int(((peer->GetSendtoCounter() - last_sendto_counter) *
//                           peer->GetChunkSize() * 8) / 1000.0f);
//        last_sendto_counter = peer->GetSendtoCounter();
//        kbps_recvfrom = int(((peer->GetRecvfromCounter() - last_recvfrom_counter) *
//                             peer->GetChunkSize() * 8) / 1000.0f);
//        last_recvfrom_counter = peer->GetRecvfromCounter();

//#if not defined __IMS__

//        kbps_expected_recv = int(((peer->GetPlayedChunk() - last_chunk_number) *
//                                  peer->GetChunkSize() * 8) / 1000.0f);
//        last_chunk_number = peer->GetPlayedChunk();
//        {
//            team_ratio = peer->GetPeerList()->size() / (peer->GetPeerList()->size() + 1.0f);
//        }
//        kbps_expected_sent = (int)(kbps_expected_recv * team_ratio);

//        if (kbps_expected_recv < kbps_recvfrom) {
//            std::cout << _SET_COLOR(_GREEN);
//        } else if (kbps_expected_recv > kbps_recvfrom) {
//            std::cout << _SET_COLOR(_RED);
//        }

//#endif /* not defined __IMS__ */

//        std::cout
//                << std::setw(9)
//                << kbps_recvfrom
//                << _RESET_COLOR();

//#if not defined __IMS__

//        std::cout
//                << std::setw(9)
//                << kbps_expected_recv;

//        std::cout
//                << " |";

//        if (kbps_expected_sent < kbps_sendto) {
//            std::cout << _SET_COLOR(_GREEN);
//        } else if (kbps_expected_sent > kbps_sendto) {
//            std::cout << _SET_COLOR(_RED);
//        }

//#endif /* not defined __IMS__ */

//        std::cout
//                << std::setw(9)
//                << kbps_sendto
//                << _RESET_COLOR();

//#if not defined __IMS__

//        std::cout
//                << std::setw(9)
//                << kbps_expected_sent;

//#endif /* not defined __IMS__ */

//        std::cout << " |";

//#ifndef __IMS__

//        {
//            std::cout
//                    << std::setw(5)
//                    << peer->GetPeerList()->size()
//                    << " | ";
//            counter = 0;
//            for (std::vector<boost::asio::ip::udp::endpoint>::iterator p = peer->GetPeerList()->begin();
//                    p != peer->GetPeerList()->end();
//                    ++p) {
//                if (counter < 5) {
//                    std::cout << p->address().to_string()
//                              << ","
//                              << std::to_string(p->port())
//                              << " ";
//                    counter++;
//                } else {
//                    break;
//                    std::cout << "";
//                }
//            }
//        }

//#endif
//        std::cout
//                << std::endl;
//    }

//    return 0;
//}

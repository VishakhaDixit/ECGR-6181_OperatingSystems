#include "thread_pool_server.hpp"

threadPoolServer::threadPoolServer(int maxThreads)
    : server{}
{
    //Create specified number of threads in the pool
    for(int i = 0; i < maxThreads; i++)
    {
        mThreads.emplace_back(&threadPoolServer::execute_thread, this, i);
    }
}

void threadPoolServer::run()
{
    create_server_sock();

    while (true)
    {
        int client_fd = wait_for_client();
        if(client_fd != -1)
        {
            connQueue.push(client_fd);
            mSignal.notify_one();
        }
    }
}

void threadPoolServer::execute_thread(int id)
{
    cout << endl << "Thread "<< id << ": Spawned"<<endl;
    while (true)
    {
        std::unique_lock<std::mutex> lk{connLock};
        cout << "Thread "<< id << ": Sleeping"<<endl;
        mSignal.wait(lk);
        cout << "Thread "<< id << ": Woken Up"<<endl;

        if(connQueue.empty())
        {
            cout << "Thread "<< id << ": No connections in queue"<<endl;
            continue;
        }
        else
        {
            int client_fd = connQueue.front();
            connQueue.pop();
            cout << "Thread "<< id << ": Processing Image."<<endl;
            cv::Mat serverImg = recieve_image(client_fd);
            std::this_thread::sleep_for(std::chrono::seconds{ 10 });
            cv::Mat greyImg   = process_image(serverImg);
            send_image(client_fd, greyImg);
            cout << "Thread "<< id << ": Processed Image."<<endl;
        }
    }
    
}

void threadPoolServer::create_server_sock()
{
    std::cout << "MainServer: Main server started" << std::endl;
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    }

    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    serverAddr.sin_port = htons( PORT ); 

    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&serverAddr,  
                                sizeof(serverAddr))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    // Listen to on the addr and port
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
}

int threadPoolServer::wait_for_client()
{
    std::cout << "MainServer: Socket server setup. Waiting for connections" << std::endl; 

    // if we get a new connection request
    int clientAddrSize = sizeof(clientAddr);
    int client_fd;
    if((client_fd = accept(server_fd, (struct sockaddr *)&clientAddr, (socklen_t *)&clientAddrSize)))
    {
        cout << "Client Connected!" << std::endl;
        return client_fd;
    }
    else
    {
        cout << "Client Connection Failed!" << std::endl;
        return -1;
    }
}
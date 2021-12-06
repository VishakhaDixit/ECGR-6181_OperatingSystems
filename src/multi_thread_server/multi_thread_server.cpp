#include "multi_thread_server.hpp"

int multiThreadServer::threadId = 0;

void multiThreadServer::run()
{
    create_server_sock();
    while(1)
    {
        int clientSock = wait_for_client();
        if(clientSock != -1)
        {
            create_thread(clientSock);
        }
        // for(auto it : mThreads)
        // {
        //     ThreadData data;
        //     data = it.second;
        //     if(data.finished.load())
        //     {
        //         data.thread.join();
        //     }
        // }
    }
}

void multiThreadServer::destroy_thread()
{
    //mThread.join();
}

void multiThreadServer::create_thread(int client_fd)
{
    int id = threadId++;

    ThreadData *td = nullptr;

    {
        std::lock_guard<std::mutex> lock{ mMutex };
        td = &mThreads[id];
    }

    td->finished.store(false);
    td->thread = std::thread{&multiThreadServer::execute_thread, this, id, client_fd};

    cout << "Thread Created: " << id << std::endl;
}

void multiThreadServer::execute_thread(int id, int client_fd)
{
    std::this_thread::sleep_for(std::chrono::seconds{ 10 });

    cv::Mat serverImg = recieve_image(client_fd);
    cv::Mat greyImg   = process_image(serverImg);
    send_image(client_fd, greyImg);
    
    {
        std::lock_guard<std::mutex> lock{ mMutex };
        mThreads[id].finished.store(true);
    }

    cout << "Thread: "<<id<< " Finished"<<std::endl;
}

void multiThreadServer::create_server_sock()
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

int multiThreadServer::wait_for_client()
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
#include "thread_pool_server.hpp"

static int conn_count = 0;

threadPoolServer::threadPoolServer(int maxThreads)
    : server{}
{
    std::cout << std::endl << "Creating Pool of threads" << std::endl;

    //Create specified number of threads in the pool
    for(int i = 0; i < maxThreads; i++)
    {
        mThreads.emplace_back(&threadPoolServer::execute_thread, this, i);
    }
}

void threadPoolServer::run()
{
    create_server_sock(THREAD_POOL_SERVER_PORT);

    while (true)
    {
        int client_fd = wait_for_client();
        if(client_fd != -1)
        {
            queue.push_back(client_fd);
        }
    }
}

void threadPoolServer::execute_thread(int id)
{
    cv::Mat convertedImg;
    std::pair<cv::Mat, uint8_t> p;

    cout << endl << "Thread "<< id << ": Spawned"<<endl;
    while (true)
    {
        int client_fd = queue.front();
        queue.pop_front();
        cout <<endl << "Thread "<< id << ": Processing Image."<<endl;
        p = recieve_image(client_fd);
        // std::this_thread::sleep_for(std::chrono::seconds{ 10 });
        convertedImg = process_image(p.first, p.second - 48);
        send_image(client_fd, convertedImg);
        cout <<endl << "Thread "<< id << ": Processed Image."<<endl;
    }
}

void threadPoolServer::create_server_sock(int port)
{
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    }

    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    serverAddr.sin_port = htons(port); 

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
    std::cout << std::endl << "Waiting for connections" << std::endl; 

    // if we get a new connection request
    int clientAddrSize = sizeof(clientAddr);
    int client_fd;
    if((client_fd = accept(server_fd, (struct sockaddr *)&clientAddr, (socklen_t *)&clientAddrSize)))
    {
        conn_count++;
        cout << std::endl << "Client " << conn_count << " Connected!" << std::endl;
        return client_fd;
    }
    else
    {
        conn_count++;
        cout << "Client " << conn_count << " Connection Failed!" << std::endl;
        return -1;
    }
}
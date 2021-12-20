# ECGR-6181_OperatingSystems

![image](https://user-images.githubusercontent.com/87341533/146835051-b748dfd7-82b4-472b-8b19-76615537eccc.png)
<p align="center">
Figure 1: Class diagram of server
</p>

### Client: 
Establishes connection with server and sends the image it wants to process along with the choice of filter. It receives filtered image from server and displays both sent and received images in a display window. It uses base64 encoding and decoding format because binary data contains null character which mean end of string, so entire data may not be read by the receiver. Base64 encodes binary data in base64 using base64 look up table.

### Server: 
Base class for different types of servers. It has a pure virtual function “run()” as each sub class has different approach for handling the client connections and processing the client request. Server has functions for receiving image from client, processing the image based on filter option provided, and sending the processed image to client. 

### simpleServer: 
No threads. Single execution. Single connection.

### multiThreadServer: 
New thread created for each connection. Server keeps listening for conn. Multiple clients can connect simultaneously. Each connection handled by a thread. A database maintained for threads by the server as an unordered map. New connections accepted as a thread is already processing. But the execution of each new thread can only begin when current thread completes its execution. Used std::thread for thread creation, it is extension of pthread for c++. Using 10 seconds delay to simulate load.

### threadPoolServer: 
It creates predefined number of threads in the constructor and stores it in an array created by standard vector. Server runs in the process context. Every time a new connection comes the server pushes the connection requests in a queue. Standard queue was used for this purpose. Since the standard queue is not thread safe, a separate class for queue operation is maintained, this class uses mutex lock before any operation (push, pop, size) can be performed on the queue. This thread safe queue is basically bounded to a limit of twice the number of worker threads in the server. This limit is added to avoid starvation of client requests.
Server allocates these connection requests to the thread as and when threads are available and have finished its current execution. Thread pops the clients socket information and processes it. Since there is no priority assigned to threads, there will be no preemption.

### threadPoolPriorityServer: 
Sever itself is a thread and has higher priority than the worker threads. This allows process to spawn multiple server thread on different IP addresses or different ports. Schedular follows a policy to assign priority to threads. Our goal is to define policy for the schedular in such a way that worker threads have lesser priority than master thread (server). Since in this case all the work is done using threads, the main process can reach the end of its main function, so to keep the process alive and the threads running we use join. Pthread affinity is used to assign each thread to a core, this introduces concurrency and makes all the worker threads run parallelly. 

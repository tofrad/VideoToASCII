#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <windows.h>


using namespace cv;

std::string ASCII_Chars = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/()1{}[]?-_+~<>i!lI;:,. ";

std::string img_path = "Budapest.jpeg";

std::string vid_path = "train.mp4";

std::string output_file = "ASCII.txt";

int mode = 0; //0 = video from cam, 1 video from file, 2 image from file

void getConsoleSize(int& width, int& height) {

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

//Fills the string buffer with content from the given Mat Object
void ConvertMatString(Mat img, std::string& buf){

    Mat gry;
    cvtColor(img, gry, COLOR_BGR2GRAY);

    for (int i = 0; i < gry.rows; i++) {

        for (int j = 0; j < gry.cols; j++) {

            int pixel = (int)gry.at<uchar>(i, j);

            int char_idx = std::clamp((int)(pixel / ASCII_Chars.size()), 0, (int)ASCII_Chars.size());
            buf += ASCII_Chars[char_idx];

        }
        buf += "\n";
    }
}
//Funtion to fit and stream a Mat to Console
void DisplayFrame(Mat fr) {

    std::string outbuffer;

    int console_width, console_height;
    float aspect = 0.45f;
    //get console size and fit output
    getConsoleSize(console_width, console_height);

    int targetHeight = static_cast<int>(console_width * aspect * fr.rows / fr.cols);

    targetHeight = (std::min)(targetHeight, console_height - 2);

    Mat resized;
    resize(fr, resized, Size(console_width, console_height));

    ConvertMatString(resized, outbuffer);

    std::cout << outbuffer;
    outbuffer = "";

}

int main()
{

    Mat frame;
    VideoCapture cap;

    std::string outbuffer;
    //video from cam
    if (mode == 0){

        cap.open(0);

        while (true) {

            cap >> frame;

            DisplayFrame(frame);

        }
    }
    //video from file
    if (mode == 1){

        VideoCapture cap(vid_path);

        while (true) {

            cap >> frame;

            if (frame.empty()) break;

            DisplayFrame(frame);
        }

    }
    //static image from file
    if (mode == 2){

        std::ofstream output(output_file);

        Mat image = imread(img_path);

        Mat resized;
        resize(frame, resized, Size(640, 360)); //640 x 360

        ConvertMatString(resized, outbuffer);

        output << outbuffer;
    }

    return 0;
}

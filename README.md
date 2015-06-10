To Install use Jetson Hacks
LINK: http://jetsonhacks.com/2014/11/26/nvidia-jetson-tk1-install-opencv-l4t-21-1/
Git Repository: https://gist.github.com/jetsonhacks/7a84e3c4b7c3b7fc25d8
--Library Used is 
--libopencv4tegra-repo_l4t-r21_2.4.10.1_armhf.deb

Important :=
	Cuda must be installed(Jetsonhacks Guide).
	Library Was Upgraded.
	Problem was Imshow and cvwaitKey.
	Samples Of This Library(2.4.11)[Available at OpenCV Git Sorce] Work Well.
Problem := 
	The Library installed as shown in above tutorial has problem with few graphics functions like imshow().
Solution:=
	It was again downloaded to next available version (...21.2) and installed.[SOLVED]

Command To Run codes: 
g++ -L /usr/local/cuda/lib VideoThreshold.cpp -o threshold.out `pkg-config --cflags --libs opencv`

Alternate Command:
g++ Videocapture.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lopencv_gpu -o a.out

For bgfg_segmp.cpp
Compile: g++ -L /usr/local/cuda/lib bgfg_segmp.cpp -o a.out `pkg-config --cflags --libs opencv`
Run: ./a.out
Press Space to Toggle the Background Updation.

For farneback_optical_flow.cpp
Compile: g++ -L /usr/local/cuda/lib farneback_optical_flow.cpp -o a.out `pkg-config --cflags --libs opencv`
Run: ./a.out -l basketball1.png -r basketball2.png
On The Output Image Press 'm' To Toggle Between CPU and GPU.

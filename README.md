To Install use Jetson Hacks
--Library Used is 
--libopencv4tegra-repo_l4t-r21_2.4.10.1_armhf.deb

Important =>
	Library Was Upgraded to Latest.
	Problem was Imshow and cvwaitKey.

Command To Run codes -- 
g++ -L /usr/local/cuda/lib VideoThreshold.cpp -o threshold.out `pkg-config --cflags --libs opencv`

Alternate Command --
g++ Videocapture.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lopencv_gpu -o a.out

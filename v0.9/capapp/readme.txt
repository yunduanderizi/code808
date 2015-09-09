===============================================================
VPU Encoding Test Program
===============================================================
실행방법
./dual_encode -i cam 1920 1080 -i cam2 640 480
option
-i : thread name(아무거나 상관없음)
-i의 갯수만큼 thread 가 만들어져서 encoding test를 진행함
-i의 갯수는 최대 2개임

-v : h264 or mpeg4, video codec, H.264와 MPEG4를 선택(default h264)

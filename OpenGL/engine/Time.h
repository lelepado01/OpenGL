//
//  Time.hpp
//  OpenGL
//
//  Created by Gabriele Padovani on 18/11/21.
//

#ifndef Time_h
#define Time_h

#include <stdio.h>
#include <chrono>

class Time {
private:
    static float deltaTime; 
    static long frameCount;
    
    static std::chrono::time_point<std::chrono::steady_clock> oldTime;
    static std::chrono::time_point<std::chrono::steady_clock> newTime;
    
public:
    static float DeltaTime();
    static void Count();
    
    static long GetFrameCount();
    static long GetMillisecondsFromEpoch();
    static long GetMicrosecondsFromEpoch();

};

#endif /* Time_h */

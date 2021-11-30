//
//  Time.c
//  OpenGL
//
//  Created by Gabriele Padovani on 18/11/21.
//

#include "Time.h"


float Time::deltaTime = 0;
long Time::frameCount = 0;

std::chrono::time_point<std::chrono::steady_clock> Time::oldTime = std::chrono::steady_clock::now();
std::chrono::time_point<std::chrono::steady_clock> Time::newTime = std::chrono::steady_clock::now();

float Time::DeltaTime(){
    return deltaTime <= 0 || deltaTime > 10000 ?  0.001 : deltaTime;
}

void Time::Count(){
    frameCount += 1;
    
    oldTime = newTime;
    newTime = std::chrono::steady_clock::now();
    
    deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - oldTime).count() / 1000.0f;
}

long Time::GetMillisecondsFromEpoch(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

long Time::GetMicrosecondsFromEpoch(){
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

long Time::GetFrameCount(){
    return frameCount; 
}

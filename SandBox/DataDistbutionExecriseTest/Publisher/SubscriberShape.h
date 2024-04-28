#pragma once

#include <string>
#include <vector>
#include <cmath> // Include cmath for floor function
#include "SendingInfo.h" 

class SubscriberShape {
public:
    SubscriberShape(const std::string& shapeType, int frequencyInHZ);

    bool checkUpdateTime(int time);

    // Other member functions...


    std::string shapeType;
    double frequency;
    std::vector<SendingInfo> specificTypeList;

    double convertFrequency(int frequencyInHZ);
};

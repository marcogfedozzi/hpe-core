/* BSD 3-Clause License

Copyright (c) 2021, Event Driven Perception for Robotics
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#pragma once

#include <array>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <deque>

namespace hpecore {

typedef struct pixel_event
{
    int p:1;
    int x:10;
    int _f1:1;
    int y:9;
    int _f:11;
    int stamp:32;
} pixel_event;

typedef struct point_flow
{
    float udot;
    float vdot;
} point_flow;

using skeleton = std::vector<std::tuple<double, double>>;

typedef struct {
    float u;
    float v;
} joint ;

typedef std::array<joint, 13> sklt;
enum skltJoint {head, shoulderR, shoulderL, elbowR, elbowL,
             hipL, hipR, handR, handL, kneeR, kneeL, footR, footL};

inline skltJoint str2enum(const std::string& str)
{
    if(str == "head") return head;
    else if(str == "shoulderR") return shoulderR;
    else if(str == "shoulderL") return shoulderL;
    else if(str == "elbowR") return elbowR;
    else if(str == "elbowL") return elbowL;
    else if(str == "hipL") return hipL;
    else if(str == "hipR") return hipR;
    else if(str == "handR") return handR;
    else if(str == "handL") return handL;
    else if(str == "kneeR") return kneeR;
    else if(str == "kneeL") return kneeL;
    else if(str == "footR") return footR;
    else if(str == "footL") return footL;
}

typedef std::array<joint, 13> skeleton13;
typedef std::array<joint, 18> skeleton18;

template <typename T>
inline void getEventsUV(std::deque<T> &input, std::deque<joint> &output, std::deque<double> &ts, double scaler) 
{
    for (auto &q : input)
    {
        joint j = {q.x, q.y};
        output.push_back(j);
        ts.push_back(q.stamp*scaler);
    }
}

// mapping between DHP19 joints and indices of joints generated by openpose using the COCO body model
enum DHP19_TO_COCO_INDICES {
    head_coco=0,  // no head in coco, use nose
    shoulderR_coco=2,
    shoulderL_coco=5,
    elbowR_coco=3,
    elbowL_coco=6,
    hipL_coco=11,
    hipR_coco=8,
    handR_coco=4,  // no right hand in coco, use right wrist
    handL_coco=7,  // no left hand in coco, use left wrist
    kneeR_coco=9,
    kneeL_coco=12,
    footR_coco=10,  // no right foot in coco, use right ankle
    footL_coco=13  // no left foot in coco, use left ankle
};

// mapping between DHP19 joints and indices of joints generated by openpose using the BODY_25 body model
enum DHP19_TO_BODY25_INDICES {
    head_body25=0,  // no head in body25, use nose
    shoulderR_body25=2,
    shoulderL_body25=5,
    elbowR_body25=3,
    elbowL_body25=6,
    hipL_body25=12,
    hipR_body25=9,
    handR_body25=4,  // no right hand in body25, use right wrist
    handL_body25=7,  // no left hand in body25, use left wrist
    kneeR_body25=10,
    kneeL_body25=13,
    footR_body25=11,  // no right foot in body25, use right ankle
    footL_body25=14  // no left foot in body25, use left ankle
};

        
inline void print_skeleton(const skeleton s) 
{
    for(auto &t : s)
        std::cout << std::get<0>(t) << " " << std::get<1>(t) << std::endl;
}

inline void print_sklt(const sklt s) 
{
    for(auto& j : s)
        std::cout << j.u << " " << j.v << std::endl;
}

inline skeleton13 coco18_to_dhp19(const skeleton skeleton_in)
{

    skeleton13 skeleton_out;
    if(skeleton_in.size() != 18) 
    {
        if(skeleton_in.size() > 0)
            std::cerr << "Converting 18 joint to 13 joint: input not 18 joints" << std::endl;
        return skeleton_out;
    }

    skeleton_out[0] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::head_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::head_coco))};
    skeleton_out[1] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::shoulderR_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::shoulderR_coco))};
    skeleton_out[2] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::shoulderL_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::shoulderL_coco))};
    skeleton_out[3] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::elbowR_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::elbowR_coco))};
    skeleton_out[4] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::elbowL_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::elbowL_coco))};
    skeleton_out[5] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::hipL_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::hipL_coco))};
    skeleton_out[6] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::hipR_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::hipR_coco))};
    skeleton_out[7] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::handR_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::handR_coco))};
    skeleton_out[8] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::handL_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::handL_coco))};   
    skeleton_out[9] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::kneeR_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::kneeR_coco))};
    skeleton_out[10] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::kneeL_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::kneeL_coco))};
    skeleton_out[11] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::footR_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::footR_coco))};
    skeleton_out[12] = {std::get<0>(skeleton_in.at(DHP19_TO_COCO_INDICES::footL_coco)), 
                       std::get<1>(skeleton_in.at(DHP19_TO_COCO_INDICES::footL_coco))};                  


    return skeleton_out;
}

inline skeleton body25_to_dhp19(const skeleton skeleton_in)
{
    skeleton skeleton_out;

    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::head_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::head_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::shoulderR_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::shoulderR_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::shoulderL_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::shoulderL_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::elbowR_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::elbowR_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::elbowL_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::elbowL_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::hipL_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::hipL_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::hipR_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::hipR_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::handR_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::handR_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::handL_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::handL_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::kneeR_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::kneeR_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::kneeL_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::kneeL_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::footR_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::footR_body25))));
    skeleton_out.push_back(std::make_tuple(std::get<0>(skeleton_out.at(DHP19_TO_BODY25_INDICES::footL_body25)), std::get<1>(skeleton_out.at(DHP19_TO_BODY25_INDICES::footL_body25))));

    return skeleton_out;
}

}

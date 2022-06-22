/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ORBEXTRACTOR_H
#define ORBEXTRACTOR_H

#include <vector>
#include <list>
#include <opencv/cv.h>

#include <opencv2/xfeatures2d.hpp>
#include "opencv2/imgcodecs/legacy/constants_c.h"
#include <opencv2/highgui/highgui_c.h>

#include <torch/script.h> // One-stop header.
#include <torch/torch.h>

using namespace cv;
using namespace std;
namespace ORB_SLAM2
{

class ExtractorNode
{
public:
    ExtractorNode():bNoMore(false){}

    void DivideNode(ExtractorNode &n1, ExtractorNode &n2, ExtractorNode &n3, ExtractorNode &n4);

    std::vector<cv::KeyPoint> vKeys;
    cv::Point2i UL, UR, BL, BR;
    std::list<ExtractorNode>::iterator lit;
    bool bNoMore;
};

class ULNCextractor
{
public:
    
    enum {HARRIS_SCORE=0, FAST_SCORE=1 };

    ULNCextractor(int nfeatures, float scaleFactor, int nlevels,
                 int iniThFAST, int minThFAST);

    ~ULNCextractor(){}

    // Compute the ULNC features and descriptors on an image.
    
    void operator()( cv::InputArray _image, cv::InputArray _mask, cv::InputArray _conf,
      std::vector<cv::KeyPoint>& keypoints,
      cv::OutputArray descriptors);

    int inline GetLevels(){
        return nlevels;}

    float inline GetScaleFactor(){
        return scaleFactor;}

    std::vector<float> inline GetScaleFactors(){
        return mvScaleFactor;
    }

    std::vector<float> inline GetInverseScaleFactors(){
        return mvInvScaleFactor;
    }

    std::vector<float> inline GetScaleSigmaSquares(){
        return mvLevelSigma2;
    }

    std::vector<float> inline GetInverseScaleSigmaSquares(){
        return mvInvLevelSigma2;
    }

    std::vector<cv::Mat> mvImagePyramid;
    std::vector<cv::Mat> mvImagePyramid_color;
    std::vector<cv::Mat> mvImagePyramid_conf;

protected:

    void ComputePyramid(cv::Mat image);
    void ComputePyramid_color(cv::Mat image);
    void ComputePyramid_conf(cv::Mat image);
    void ComputeKeyPointsOctTree(std::vector<std::vector<cv::KeyPoint> >& allKeypoints);    
    std::vector<cv::KeyPoint> DistributeOctTree(const std::vector<cv::KeyPoint>& vToDistributeKeys, const int &minX,
                                           const int &maxX, const int &minY, const int &maxY, const int &nFeatures, const int &level);

    void searchKPbyNN(Mat imgCell, Mat conf, vector<cv::KeyPoint>& vKeysCell, int Th);
    
    std::vector<cv::Point> pattern;
    cv::Mat descriptors;

    int nfeatures;
    double scaleFactor;
    int nlevels;
    int iniThFAST;
    int minThFAST;
    
    int downsample=8;
    const int NN_Th = 0.9;
    const int MIN_POINT_NUM = 200;
    const int BLOCK_NUM = 4;
    const int COL_NUM = 2;
    bool FILL_FLAG = 1;

    std::vector<int> mnFeaturesPerLevel;

    std::vector<int> umax;

    std::vector<float> mvScaleFactor;
    std::vector<float> mvInvScaleFactor;    
    std::vector<float> mvLevelSigma2;
    std::vector<float> mvInvLevelSigma2;
    
    torch::jit::script::Module module;
};

} 

#endif


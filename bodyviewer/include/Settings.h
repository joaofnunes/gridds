#pragma once
#include <opencv2/opencv.hpp>

#define PI 3.14159265

enum TrackingState { 
    TrackingState_NotTracked = 0, 
    TrackingState_Inferred = 1, 
    TrackingState_Tracked = 2
};

enum JointType { 
    JointType_SpineBase = 0, 
    JointType_SpineMid = 1, 
    JointType_Neck = 2, 
    JointType_Head = 3, 
    JointType_ShoulderLeft = 4, 
    JointType_ElbowLeft = 5, 
    JointType_WristLeft = 6, 
    JointType_HandLeft = 7, 
    JointType_ShoulderRight = 8, 
    JointType_ElbowRight = 9, 
    JointType_WristRight = 10, 
    JointType_HandRight = 11, 
    JointType_HipLeft = 12, 
    JointType_KneeLeft = 13, 
    JointType_AnkleLeft = 14, 
    JointType_FootLeft = 15, 
    JointType_HipRight = 16, 
    JointType_KneeRight = 17, 
    JointType_AnkleRight = 18, 
    JointType_FootRight = 19, 
    JointType_SpineShoulder = 20, 
    JointType_HandTipLeft = 21, 
    JointType_ThumbLeft = 22, 
    JointType_HandTipRight = 23, 
    JointType_ThumbRight = 24 
};

enum PlayMode {
	Pause = 0,
	StepForward = 1,
	StepBacward = -1,
	Run = 2
};

//Variables for skeleton
static const int	c_JointType_Count = 25;
static const float	c_JointThickness = 2.0f;
static const int	c_TrackedBoneThickness = 3;
static const int	c_InferredBoneThickness = 1;
static const float	c_HandSize = 30.0f;
static const int	c_jointRadius = 2;

static const cv::Scalar g_colorTrackedBone = cv::Scalar(4, 119, 8); //dark green
static const cv::Scalar g_colorTrackedBoneLeft = cv::Scalar(255, 188, 0); //dark orange
static const cv::Scalar g_colorTrackedBoneRight = cv::Scalar(0, 162, 255); //dark blue
static const cv::Scalar g_colorTrackedBoneFiltered = cv::Scalar(102, 255, 108); //light green

static const cv::Scalar g_colorTrackedJoint = cv::Scalar(178, 227, 255); //light blue
static const cv::Scalar g_colorTrackedJointFiltered = cv::Scalar(0, 162, 255); //dark blue

static const cv::Scalar g_colorInferredBone = cv::Scalar(252, 255, 151); //light yellow
static const cv::Scalar g_colorInferredBoneLeft = cv::Scalar(252, 255, 151); //light yellow
static const cv::Scalar g_colorInferredBoneRight = cv::Scalar(252, 255, 151); //light yellow
static const cv::Scalar g_colorInferredBoneFiltered = cv::Scalar(255, 255, 0); //dark yellow

static const cv::Scalar g_colorInferredJoint = cv::Scalar(255, 220, 119); //light orange
static const cv::Scalar g_colorInferredJointFiltered = cv::Scalar(255, 188, 0); //dark orange

static const cv::Scalar g_colorNonTrackedJoint = cv::Scalar(148, 160, 255);



//Frame dimensions
const int g_depth_row = 424;			//Row in depth frame
const int g_depth_col = 512;			//Column in depth frame
const int g_color_row = 1080;			//Row in color frame
const int g_color_col = 1920;			//Column in color frame
const int g_mapp_col = 512*2;			//Column in mapp table

// //Struct for body frame
// typedef struct 
// {
// 	Vector4 body[JointType_Count];
// 	int jointState[JointType_Count];
// }   Body;
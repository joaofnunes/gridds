#include <iostream>
#include "JointData.h"

using namespace std;


//constructor
JointData::JointData()
{
}

JointData::JointData(JointType jointType, TrackingState jointTrackingState, double posXCameraSP, double posYCameraSP, double posZCameraSP)
{
	mJointType = jointType;
	mJointTrackingState = jointTrackingState;
	mPosXCameraSP = posXCameraSP;
	mPosYCameraSP = posYCameraSP;
	mPosZCameraSP = posZCameraSP;
}


void JointData::setJointType(JointType type)
{
	mJointType = type;
}
	
const JointType JointData::getJointType() const
{
	return mJointType;
}

	
	
void JointData::setJointTrackingState(TrackingState state)
{
	mJointTrackingState = state;
}
	
const TrackingState JointData::getJointTrackingState() const
{
	return mJointTrackingState;
}
	
	
	
void JointData::setPosXCameraSP(double posX)
{
	mPosXCameraSP = posX;
}
	
const double JointData::getPosXCameraSP() const
{
	return mPosXCameraSP;
}


void JointData::setPosYCameraSP(double posY)
{
	mPosYCameraSP = posY;
}
	
const double JointData::getPosYCameraSP() const
{
	return mPosYCameraSP;
}


void JointData::setPosZCameraSP(double posZ)
{
	mPosZCameraSP = posZ;
}
	
const double JointData::getPosZCameraSP() const
{
	return mPosZCameraSP;
}
	
void JointData::setPosXColorSP(double posX)
{
	mPosXColorSP = posX;
}

const double JointData::getPosXColorSP() const
{
	return mPosXColorSP;
}


void JointData::setPosYColorSP(double posY)
{
	mPosYColorSP = posY;
}

const double JointData::getPosYColorSP() const
{
	return mPosYColorSP;
}

void JointData::setPosXDepthSP(double posX)
{
	mPosXDepthSP = posX;
}

const double JointData::getPosXDepthSP() const
{
	return mPosXDepthSP;
}


void JointData::setPosYDepthSP(double posY)
{
	mPosYDepthSP = posY;
}

const double JointData::getPosYDepthSP() const
{
	return mPosYDepthSP;
}

	
const std::string JointData::toString() const
{
	std::string str;
	//= "JOINT :: JType (" + std::to_string(mJointType) + "): " + std::to_string(mPosXColorSP) + ", " + std::to_string(mPosYColorSP);
	return str;
}

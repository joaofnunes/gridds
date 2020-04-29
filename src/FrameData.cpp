#include <iostream>
#include "FrameData.h"

using namespace std;

//constructor
FrameData::FrameData()
{
	mFrameNumber = -1;
}


void FrameData::setFrameNumber(int frame)
{
	mFrameNumber = frame;
}
	
int FrameData::getFrameNumber() const
{
	return mFrameNumber;
}	
	
void FrameData::setFrameValue(int frame)
{
	mFrameValue = frame;
}
	
int FrameData::getFrameValue() const
{
	return mFrameValue;
}


void FrameData::setFrameBodyJoints(vector<JointData> body)
{
	mFrameBodyJoints = body;
}

vector<JointData> FrameData::getFrameBodyJoints()
{
	return mFrameBodyJoints;
}

void FrameData::setFrameBodyTrackingStatus(bool state)
{
	mFrameBodyTrackingStatus = state;
}
	
bool FrameData::isFrameBodyTracked() const
{
	return mFrameBodyTrackingStatus;
}

void FrameData::setCEBottom(int state)
{
	mCEBottom = state;
}
	
int FrameData::getCEBottom() const
{
	return mCEBottom;
}

void FrameData::setCETop(int state)
{
	mCETop = state;
}
	
int FrameData::getCETop() const
{
	return mCETop;
}
	
void FrameData::setCELeft(int state)
{
	mCELeft = state;
}
	
int FrameData::getCELeft() const
{
	return mCELeft;
}
	
void FrameData::setCERight(int state)
{
	mCERight = state;
}
	
int FrameData::getCERight() const
{
	return mCERight;
}
	
const std::string FrameData::toString() const
{
	//std::string str = "#" + to_string(mFrameNumber) + "\n";
	std::string str = "";
	for(int i = 0; i < mFrameBodyJoints.size(); i++)
		str = str + mFrameBodyJoints.at(i).toString();
	return str;
}
#pragma once

#include <vector>
#include "JointData.h"

using namespace std;

class FrameData
{
    public:
		FrameData();
		
		//FrameData(double frameNumber, double frameValue, int cEBottom, int cETop, int cELeft, int cERight, BodyData body);
			
		void				setFrameNumber(int frame);
		int					getFrameNumber() const;
			
		void				setFrameValue(int frame);
		int					getFrameValue() const;
			
		void				setFrameBodyJoints(vector<JointData> body);
		vector<JointData>	getFrameBodyJoints();

		void				setFrameBodyTrackingStatus(bool state);
		bool				isFrameBodyTracked() const;

		void				setCEBottom(int state);
		int					getCEBottom() const;	
			
		void				setCETop(int state);
		int					getCETop() const;	
		
		void				setCELeft(int state);
		int					getCELeft() const;	
						
		void				setCERight(int state);
		int					getCERight() const;	
	
		const string		toString() const;

			
	private:

		int					mFrameNumber;
		int					mFrameValue;
	
		vector<JointData>	mFrameBodyJoints;
		bool				mFrameBodyTrackingStatus;
		
		int					mCEBottom;
		int					mCETop;
		int					mCELeft;
		int					mCERight;
};
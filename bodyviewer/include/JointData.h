#pragma once

#include "Settings.h"


class JointData
{
    public:
		JointData();

		JointData(JointType jointType, TrackingState jointTrackingState, double posXCameraSP, double posYCameraSP, double posZCameraSP);

		void						setJointType(JointType type);
		const JointType				getJointType() const;

		void						setJointTrackingState(TrackingState state);
		const TrackingState			getJointTrackingState() const;
			
		void						setPosXCameraSP(double posX);
		const double				getPosXCameraSP() const;

		void						setPosYCameraSP(double posY);
		const double				getPosYCameraSP() const;

		void						setPosZCameraSP(double posZ);
		const double				getPosZCameraSP() const;

		void						setPosXColorSP(double posX);
		const double				getPosXColorSP() const;

		void						setPosYColorSP(double posY);
		const double				getPosYColorSP() const;

		void						setPosXDepthSP(double posX);
		const double				getPosXDepthSP() const;

		void						setPosYDepthSP(double posY);
		const double				getPosYDepthSP() const;

		const std::string			toString() const;
		
	private:
		
		JointType					mJointType;
		TrackingState				mJointTrackingState;
		
		// real world coordinates
		double						mPosXCameraSP;
		double						mPosYCameraSP;
		double						mPosZCameraSP;

		// screen coordinates
		double						mPosXColorSP;
		double						mPosYColorSP;

		// depth coordinates
		double						mPosXDepthSP;
		double						mPosYDepthSP;

};

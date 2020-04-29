//#include <opencv2/opencv.hpp>
//#include <string>
//#include <iomanip>
//#include <sstream>
//#include <iostream>
//
//#include "Settings.h"
//
//using namespace std;
//using namespace cv;
//
//
//
//
//
//int main(int argc, char** argv)
//{
//
//	cv::VideoCapture g_sequence;
//
//	char *griddsFolder, *sessionID, *sessionNR;
//
//
//	if (argc != 4)
//		printf("\nFailed to parse the Command Line: invalid nr of arguments!\n");
//
//	if (argc == 4) {
//		griddsFolder = argv[1];
//		sessionID = argv[2];
//		sessionNR = argv[3];
//	}
//
//	std::string folder(griddsFolder);
//	std::string sID(sessionID);
//	std::string sNR(sessionNR);
//
//    std::string suffix = ".png";
//
//	if (folder.back() != '\\')
//		folder = folder + "\\";
//	
//	folder = folder + sID + "\\";
//    int counter = 1;
//
//    cv::Mat myImage;
//
//	//cout << "folder = " << griddsFolder << endl;
//
//    while (1)
//    {
//        std::stringstream ss;
//        ss << std::setw(3) << std::setfill('0') << counter; // 000, 001, 002, etc...
//        std::string number = ss.str();
//
//		
//		std::string fileName = sID + "_" + sNR + "_color_";
//
//        std::string name = folder + fileName + number + suffix;
//		std::string name2 = folder + fileName + "\%03d" + suffix;
//
//		cout << "file = " << name << endl;
//
//
//		// convert std string to cv string
//		cv::String cvStr = cv::String(name2);
//
//		//opens the image sequence
//		g_sequence.open( cvStr );
//
//		if (!g_sequence.isOpened()) {
//			cerr << "Failed to open the image sequence!\n" << endl;
//			return 0;
//		}
//
//		int totalFrames = (int)g_sequence.get(cv::CAP_PROP_FRAME_COUNT);
//		int tmpw = (int)g_sequence.get(cv::CAP_PROP_FRAME_WIDTH);
//		int tmph = (int)g_sequence.get(cv::CAP_PROP_FRAME_HEIGHT);
//
//		cout << "cvStr = " << cvStr << "\ntotalFrames = " << totalFrames << endl;
//
//
//		myImage = cv::imread(name);
//        if (myImage.empty())
//		{
//			cout << "Couldn't open the file: " << name << endl;
//		}
//		else {
//			cv::imshow("HEYO", myImage);
//		}
//        int c = cv::waitKey(1);
//        counter++;
//    }
//    return 0;
//}



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <memory>
#include <iomanip>
#include <sstream>
#include <math.h>


#include "CSVIterator.h"
#include "FrameData.h"
#include "Settings.h"

using namespace std;
using namespace cv;



cv::VideoCapture g_sequence;

int g_appCurrentFrame = 1;
float g_scaleX, g_scaleY;

cv::Mat frame;
cv::Mat frameDst;

bool viewSkel = true;


const double pi = std::atan(1.0) * 4;

struct vect2D {
	double x;
	double y;

	// scalar product of 2 vectors 
	double operator*(vect2D& e) {
		return x * e.x + y * e.y;
	}

	// length of the vector or magnitude of the vector
	double distance() {
		return sqrt((x * x) + (y * y));
	}
};

struct vect3D {  // or use vectors
	double x;
	double y;
	double z;

	// scalar product of 2 vectors 
	double operator*(vect3D& e) {
		return x * e.x + y * e.y + z * e.z;
	}

	// length of the vector or magnitude of the vector
	double distance() {
		return sqrt((x * x) + (y * y) + (z * z));
	}
};


vect2D unit_vect2D(vect2D& v)
{
	vect2D unit;
	double magnitude = v.distance();

	unit.x = v.x / magnitude;
	unit.y = v.y / magnitude;

	return (unit);
}

double product_vect2D(vect2D& u, vect2D& v)
{
	return u.x * v.y - u.y * v.x;
}

double product_vect3D(vect3D& u, vect3D& v)
{
	// FALTA!! return u.x * v.y - u.y * v.x; 
	return 0;
}

double angle_rad2D(vect2D& u, vect2D& v)
{
	return (product_vect2D(u, v) < 0 ? -1 : 1) * acos(u * v / (u.distance() * v.distance()));
}

double angle_rad3D(vect3D& u, vect3D& v)
{
	return (product_vect3D(u, v) < 0 ? -1 : 1) * acos(u * v / (u.distance() * v.distance()));
}

double angle_deg2D(vect2D& u, vect2D& v)
{
	return angle_rad2D(u, v) / pi * 180;
}
double angle_deg3D(vect3D& u, vect3D& v)
{
	return angle_rad3D(u, v) / pi * 180;
}




/// <summary>
/// Parses the CSV File that contais the coordinates of the person's action
/// <param name="bodyAnimationData">contains all the frame's data of the full session</param>
/// </summary>
static int parseCSVFile(std::vector<FrameData> &bodyAnimationData, std::string& coordsFileName)
{
	vector<JointData> local_bodyJoints;

	int jointsCounter = 0;

	std::string str;
	int frame;


	ifstream file(coordsFileName);

	if (!file.is_open())
		return 0;



	// for each line in the csv file
	for (CSVIterator line_data(file); line_data != CSVIterator(); ++line_data)
	{
		
		//collects the current line cointaining the joint data
		JointData local_jointData;

		//joint type
		JointType castJType = (JointType)jointsCounter;
		local_jointData.setJointType(castJType);

		//frame nr
		str = (*line_data)[0];
		frame = std::stoi(str);
		
		//xCam
		str = (*line_data)[1];
		local_jointData.setPosXCameraSP(std::stod(str));

		//yCam
		str = (*line_data)[2];
		local_jointData.setPosYCameraSP(std::stod(str));

		//zCam
		str = (*line_data)[3];
		local_jointData.setPosZCameraSP(std::stod(str));

		//xColor
		str = (*line_data)[4];
		local_jointData.setPosXColorSP(std::stod(str));

		//yColor
		str = (*line_data)[5];
		local_jointData.setPosYColorSP(std::stod(str));


		//trackState
		str = (*line_data)[6];
		TrackingState castTState = (TrackingState)(std::stoi(str));
		local_jointData.setJointTrackingState(castTState);

		//saves data into a local var
		local_bodyJoints.push_back(local_jointData);


		if (jointsCounter < 25)
			jointsCounter++;

		if (jointsCounter == 25)
		{
			FrameData local_frameData;

			local_frameData.setFrameNumber(frame);
			local_frameData.setFrameBodyJoints(local_bodyJoints);
			local_frameData.setFrameValue(frame);

			bodyAnimationData.at(frame) = local_frameData;


			//clears the local variable bodyJoints vector
			local_bodyJoints.clear();

			jointsCounter = 0;
		}

		
	}
	file.close();
	return 1;
}

/// <summary>
/// Calculates the length between two joints 
/// </summary>
/// <param name="pJoints">xxxxxxxxxx</param>
/// <param name="pJointPoints">xxxxxxxxxxxxxxxxx</param>
double LengthBetweenJoints(const JointData jointA, const JointData jointB)
{
	return sqrt(
		pow(jointB.getPosXCameraSP() - jointA.getPosXCameraSP(), 2) +
		pow(jointB.getPosYCameraSP() - jointA.getPosYCameraSP(), 2) +
		pow(jointB.getPosZCameraSP() - jointA.getPosZCameraSP(), 2)
	);
}


double AngleBetweenBones(const JointData jointA, const JointData jointB, const JointData jointC)
{
	double AB, BC;

	AB = (jointB.getPosXCameraSP() - jointA.getPosXCameraSP()) + (jointB.getPosYCameraSP() - jointA.getPosYCameraSP()) + (jointB.getPosZCameraSP() - jointA.getPosZCameraSP());
	BC = (jointC.getPosXCameraSP() - jointB.getPosXCameraSP()) + (jointC.getPosYCameraSP() - jointB.getPosYCameraSP()) + (jointC.getPosZCameraSP() - jointB.getPosZCameraSP());

	return (acos((AB*BC) / (LengthBetweenJoints(jointA, jointB) * LengthBetweenJoints(jointB, jointC))));
}

double GetElipseStartingAngle(const JointData jointA, const JointData jointB, const JointData jointC)
{
	double teta;
	Point2d uBA, vBC, wBD;

	uBA.x = jointA.getPosXColorSP() - jointB.getPosXColorSP();
	uBA.y = jointA.getPosYColorSP() - jointB.getPosYColorSP();

	vBC.x = jointC.getPosXColorSP() - jointB.getPosXColorSP();
	vBC.y = jointC.getPosYColorSP() - jointB.getPosYColorSP();

	wBD.x = uBA.x + vBC.x;
	wBD.y = uBA.y + vBC.y;

	teta = atan(wBD.y / wBD.x);

	return (teta);
}

// returns a point C that is located at a distance d from point A, in the vector AB
Point2d GetElipseStartingPoint(const JointData jointA, const JointData jointB, double distance)
{
	double AB, u;
	Point2d C;

	// new point C at distance d from point A = (Ax, Ay) + d*u;

	AB = (jointB.getPosXCameraSP() - jointA.getPosXCameraSP()) + (jointB.getPosYCameraSP() - jointA.getPosYCameraSP()) + (jointB.getPosZCameraSP() - jointA.getPosZCameraSP());
	u = AB / (LengthBetweenJoints(jointA, jointB));

	C.x = jointA.getPosXCameraSP() + distance * u;
	C.y = jointA.getPosYCameraSP() + distance * u;

	return C;
}

void DrawAngle(const JointData jointA, const JointData jointB, const JointData jointC)
{
	Point center;
	double tetaABBC, tetaBABC, tetaBXBC;
	vect2D AB, BC, BA;

	TrackingState jointATState = jointA.getJointTrackingState();
	TrackingState jointBTState = jointB.getJointTrackingState();
	TrackingState jointCTState = jointC.getJointTrackingState();

	if ((jointATState != TrackingState_NotTracked) && (jointBTState != TrackingState_NotTracked) && (jointCTState != TrackingState_NotTracked))
	{
		AB.x = jointB.getPosXColorSP() - jointA.getPosXColorSP();
		AB.y = jointB.getPosYColorSP() - jointA.getPosYColorSP();

		BA.x = jointA.getPosXColorSP() - jointB.getPosXColorSP();
		BA.y = jointA.getPosYColorSP() - jointB.getPosYColorSP();

		BC.x = jointC.getPosXColorSP() - jointB.getPosXColorSP();
		BC.y = jointC.getPosYColorSP() - jointB.getPosYColorSP();


		tetaABBC = acos((AB.x * BC.x + AB.y * BC.y) / (sqrt(AB.x*AB.x + AB.y*AB.y) * sqrt(BC.x*BC.x + BC.y*BC.y))) * 180.0 / PI;

		tetaBABC = acos((BA.x * BC.x + BA.y * BC.y) / (sqrt(BA.x*BA.x + BA.y*BA.y) * sqrt(BC.x*BC.x + BC.y*BC.y))) * 180.0 / PI;

		tetaBXBC = acos((BC.x) / sqrt(BC.x*BC.x + BC.y*BC.y)) * 180.0 / PI;

		center.x = (int)(jointB.getPosXColorSP() * g_scaleX);
		center.y = (int)(jointB.getPosYColorSP() * g_scaleY);

		Scalar angleColor;

		if (jointB.getJointType() == JointType_ElbowLeft || jointB.getJointType() == JointType_KneeLeft)
			angleColor = g_colorTrackedBoneLeft;
		else
			angleColor = g_colorTrackedBoneRight;




		if (jointB.getJointType() == JointType_ElbowLeft || jointB.getJointType() == JointType_ElbowRight)
		{
			ellipse(frameDst, center, Size(12, 12), tetaBXBC, 180 + tetaABBC, 360, angleColor, -1, 8, 0);
		}
		else
		{
			ellipse(frameDst, center, Size(12, 12), tetaBXBC, 180 - tetaABBC, 0, angleColor, -1, 8, 0);
		}
	}
}

void DrawBodyAngles(const vector<JointData> pframeBodyJoints, int side)
{
	if (side == -1) //LEFT
	{
		DrawAngle(pframeBodyJoints.at(JointType_ShoulderLeft), pframeBodyJoints.at(JointType_ElbowLeft), pframeBodyJoints.at(JointType_WristLeft));
		DrawAngle(pframeBodyJoints.at(JointType_HipLeft), pframeBodyJoints.at(JointType_KneeLeft), pframeBodyJoints.at(JointType_FootLeft));
	}
	else //RIGHT
	{
		DrawAngle(pframeBodyJoints.at(JointType_ShoulderRight), pframeBodyJoints.at(JointType_ElbowRight), pframeBodyJoints.at(JointType_WristRight));
		DrawAngle(pframeBodyJoints.at(JointType_HipRight), pframeBodyJoints.at(JointType_KneeRight), pframeBodyJoints.at(JointType_FootRight));
	}
	
}



/// <summary>
/// Draws a body 
/// </summary>
/// <param name="pJoints">xxxxxxxxxx</param>
/// <param name="pJointPoints">xxxxxxxxxxxxxxxxx</param>
void DrawBodyJoints(const vector<JointData> pframeBodyJoints, int mode)
{
	Point center, centerDst;
	for (int counter = 0; counter < c_JointType_Count; ++counter)
	{
		if (counter != 11 && counter != 7 && counter != 19 && counter != 15 && counter != 21 && counter != 22 && counter != 23 && counter != 24)
		{
			center.x = (int)(pframeBodyJoints.at(counter)).getPosXColorSP();
			center.y = (int)(pframeBodyJoints.at(counter)).getPosYColorSP();

			centerDst.x = (int)(g_scaleX * center.x);
			centerDst.y = (int)(g_scaleY * center.y);

			Scalar jointColor;

			TrackingState jointTState = pframeBodyJoints.at(counter).getJointTrackingState();

			if (jointTState == TrackingState_Tracked)
			{
				jointColor = (mode == 0 ? g_colorTrackedJoint : g_colorTrackedJointFiltered);
			}
			else if (jointTState == TrackingState_Inferred)
			{
				jointColor = (mode == 0 ? g_colorInferredJoint : g_colorInferredJointFiltered);
			}

			ellipse(frameDst, centerDst, Size((int)c_JointThickness, (int)c_JointThickness), 0, 0, 360, jointColor, cv::FILLED, cv::LINE_8);
		}

	}
}


/// <summary>
/// Hides the Face 
/// </summary>
/// <param name="pJoints">xxxxxxxxxx</param>
/// <param name="pJointPoints">xxxxxxxxxxxxxxxxx</param>
void HideFace(const vector<JointData> pframeBodyJoints, int mode)
{
	Point center, centerDst;
	
	center.x = (int)(pframeBodyJoints.at(3)).getPosXColorSP();
	center.y = (int)(pframeBodyJoints.at(3)).getPosYColorSP();

	centerDst.x = (int)(g_scaleX * center.x);
	centerDst.y = (int)(g_scaleY * center.y);

	Scalar jointColor;

	TrackingState jointTState = pframeBodyJoints.at(3).getJointTrackingState();

	if (jointTState == TrackingState_Tracked)
	{
		jointColor = (mode == 0 ? g_colorTrackedJoint : g_colorTrackedJointFiltered);
	}
	else if (jointTState == TrackingState_Inferred)
	{
		jointColor = (mode == 0 ? g_colorInferredJoint : g_colorInferredJointFiltered);
	}

	ellipse(frameDst, centerDst, Size((int)c_JointThickness + 10, (int)c_JointThickness + 10), 0, 0, 360, jointColor, cv::FILLED, cv::LINE_8);
}

/// <summary>
/// Draws one bone of a body (joint to joint)
/// </summary>
/// <param name="pJoints">joint data</param>
/// <param name="pJointPoints">joint positions converted to screen space</param>
/// <param name="pJointPoints">joint positions converted to screen space</param>
/// <param name="joint0">one joint of the bone to draw</param>
/// <param name="joint1">other joint of the bone to draw</param>
void DrawBone(const vector<JointData> pframeBodyJoints, JointType jointA, JointType jointB, int mode)
{
	TrackingState jointA_TState = pframeBodyJoints.at(jointA).getJointTrackingState();
	TrackingState jointB_TState = pframeBodyJoints.at(jointB).getJointTrackingState();

	int side = 0; // -1: left, 1:right, 0: otherwise

	if ((jointA >= 4 && jointA <= 7) || (jointA >= 12 && jointA <= 15) || (jointB >= 4 && jointB <= 7) || (jointB >= 12 && jointB <= 15))
		side = -1; //left side bone

	if ((jointA >= 8 && jointA <= 11) || (jointA >= 16 && jointA <= 19) || (jointB >= 8 && jointB <= 11) || (jointB >= 16 && jointB <= 19))
		side = 1; //right side bone


	// If we can't find either of these joints, exit
	if ((jointA_TState == TrackingState_NotTracked) || (jointB_TState == TrackingState_NotTracked))
	{
		return;
	}

	// Don't draw if both points are inferred
	if ((jointA_TState == TrackingState_Inferred) && (jointB_TState == TrackingState_Inferred))
	{
		return;
	}

	Point2d pointA, pointB;
	Point pointDstA, pointDstB;

	pointA.x = pframeBodyJoints.at(jointA).getPosXColorSP();
	pointA.y = pframeBodyJoints.at(jointA).getPosYColorSP();

	pointB.x = pframeBodyJoints.at(jointB).getPosXColorSP();
	pointB.y = pframeBodyJoints.at(jointB).getPosYColorSP();

	pointDstA.x = (int)(g_scaleX * pointA.x);
	pointDstA.y = (int)(g_scaleY * pointA.y);

	pointDstB.x = (int)(g_scaleX * pointB.x);
	pointDstB.y = (int)(g_scaleY * pointB.y);

	int boneThickness;
	Scalar boneColor;

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if ((jointA_TState == TrackingState_Tracked) && (jointB_TState == TrackingState_Tracked))
	{
		boneThickness = c_TrackedBoneThickness;
		//boneColor = (mode == 0 ?  g_colorTrackedBone : g_colorTrackedBoneFiltered);
		switch (side) {
		case -1:
			boneColor = g_colorTrackedBoneLeft;
			break;
		case 0:
			boneColor = g_colorTrackedBone;
			break;
		case 1:
			boneColor = g_colorTrackedBoneRight;
			break;
		}
	}
	else
	{
		boneThickness = c_InferredBoneThickness;
		boneColor = (mode == 0 ? g_colorInferredBone : g_colorInferredBoneFiltered);
	}

	line(frameDst, pointDstA, pointDstB, boneColor, boneThickness, 8, 0);
}

/// <summary>
/// Draws the body bones (line segments) 
/// </summary>
/// <param name="pJoints">joint data</param>
/// <param name="pJointPoints">joint positions converted to screen space</param>
void DrawBodyBones(const vector<JointData> pframeBodyJoints, int mode)
{
	// Draw the bones

	// Torso
	DrawBone(pframeBodyJoints, JointType_Head, JointType_Neck, mode);
	DrawBone(pframeBodyJoints, JointType_Neck, JointType_SpineShoulder, mode);
	DrawBone(pframeBodyJoints, JointType_SpineShoulder, JointType_SpineMid, mode);
	DrawBone(pframeBodyJoints, JointType_SpineMid, JointType_SpineBase, mode);
	DrawBone(pframeBodyJoints, JointType_SpineShoulder, JointType_ShoulderRight, mode);
	DrawBone(pframeBodyJoints, JointType_SpineShoulder, JointType_ShoulderLeft, mode);
	DrawBone(pframeBodyJoints, JointType_SpineBase, JointType_HipRight, mode);
	DrawBone(pframeBodyJoints, JointType_SpineBase, JointType_HipLeft, mode);

	// Right Arm    
	DrawBone(pframeBodyJoints, JointType_ShoulderRight, JointType_ElbowRight, mode);
	DrawBone(pframeBodyJoints, JointType_ElbowRight, JointType_WristRight, mode);
	//DrawBone(pframeBodyJoints, JointType_WristRight, JointType_HandRight, mode);
	//DrawBone(pframeBodyJoints, JointType_HandRight, JointType_HandTipRight, mode);
	//DrawBone(pframeBodyJoints, JointType_WristRight, JointType_ThumbRight, mode);

	// Left Arm
	DrawBone(pframeBodyJoints, JointType_ShoulderLeft, JointType_ElbowLeft, mode);
	DrawBone(pframeBodyJoints, JointType_ElbowLeft, JointType_WristLeft, mode);
	//DrawBone(pframeBodyJoints, JointType_WristLeft, JointType_HandLeft, mode);
	//DrawBone(pframeBodyJoints, JointType_HandLeft, JointType_HandTipLeft, mode);
	//DrawBone(pframeBodyJoints, JointType_WristLeft, JointType_ThumbLeft, mode);

	// Right Leg
	DrawBone(pframeBodyJoints, JointType_HipRight, JointType_KneeRight, mode);
	DrawBone(pframeBodyJoints, JointType_KneeRight, JointType_AnkleRight, mode);
	DrawBone(pframeBodyJoints, JointType_AnkleRight, JointType_FootRight, mode);

	// Left Leg
	DrawBone(pframeBodyJoints, JointType_HipLeft, JointType_KneeLeft, mode);
	DrawBone(pframeBodyJoints, JointType_KneeLeft, JointType_AnkleLeft, mode);
	DrawBone(pframeBodyJoints, JointType_AnkleLeft, JointType_FootLeft, mode);
}

static void help(char** argv)
{
	cout << "\nUsage: " << argv[0] << " GRIDDS_ROOT_FOLDER PERSON_ID SESSION_NR\n" << endl;
}


int main(int argc, char** argv)
{
	vector<JointData>	frameBodyJoints;  			//contains the 25 body coords of the current frame
	vector<JointData>	frameBodyJointsFile02;  	//contains the 25 body coords of the current frame

	vector<FrameData>   bodyAnimationData;	//contains all the frames data of the full session


	FrameData	framedata;
	JointData 	jointdata;

	//char *coordsFileName;

	char *griddsFolder, *sessionID, *sessionNR;

	if (argc != 4){
		cerr << "\nFailed to parse the Command Line: invalid number of arguments!\n" << endl;
		help(argv);
		return 0;
	}

	if (argc == 4) {
		griddsFolder = argv[1];
		sessionID = argv[2];
		sessionNR = argv[3];
	}

	// identifies the side of the body to draw the joints angles
	int side = -1;
	if (atoi(argv[3]) % 2 == 0)
		side = 1;
	
	cv::namedWindow("GRIDDS: Body Viewer", cv::WINDOW_AUTOSIZE);

	cv::CommandLineParser parser(argc, argv, "{help h||}{@image||}");
	parser.about("GRIDDS: Body Viewer v1.0");

	// parse the command line
	if (parser.has("help")) {
		help(argv);
		return 0;
	}
	
	std::string folder(griddsFolder);
	std::string sID(sessionID);
	std::string sNR(sessionNR);
	std::string suffix = ".png";

	if (folder.back() != '/')
		folder = folder + "/";

	folder = folder + sID + "/";

	
	std::string imageFileName = sID + "_" + sNR + "_color_";
	std::string imageSeqName = folder + imageFileName + "\%03d" + suffix;
	std::string coordsFileName = folder + sID + "_" + sNR + "_coordinates.csv";

	
	// convert std string to cv string
	cv::String cvStr = cv::String(imageSeqName);

	//opens the image sequence
	g_sequence.open(cvStr);

	if (!g_sequence.isOpened()) {
		cerr << "Failed to open the image sequence!\n" << endl;
		return 0;
	}

	int totalFrames = (int)g_sequence.get(cv::CAP_PROP_FRAME_COUNT);
	int tmpw = (int)g_sequence.get(cv::CAP_PROP_FRAME_WIDTH);
	int tmph = (int)g_sequence.get(cv::CAP_PROP_FRAME_HEIGHT);


	bodyAnimationData.resize(totalFrames + 1);


	
	// parse the CSV file and feed the corresponding body's animation data structure
	if (parseCSVFile(bodyAnimationData, coordsFileName) == 0)
	{
		cerr << "Failed to parse the Coordinates CSV file!\n" << endl;
		return 0;
	}

	g_scaleX = 0.5;
	g_scaleY = 0.5;
	g_appCurrentFrame = 1;

	PlayMode status = PlayMode::Pause;

	g_sequence >> frame;

	cout << "s: frame forward\na: frame backward\nr: run\nv: show/hide skeleton\nESC: quit\n" << endl;

	cout << "Current Frame = " << g_appCurrentFrame << endl;

	int frameChanged = 0;



	for (;;)
	{

		if (frame.empty()) break;

		cv::resize(frame, frameDst, Size(), g_scaleX, g_scaleY, cv::INTER_LINEAR);



		if (bodyAnimationData.at(g_appCurrentFrame).getFrameNumber() > 0)
		{
			//gets from the current frame the joints of the body 
			frameBodyJoints = bodyAnimationData.at(g_appCurrentFrame).getFrameBodyJoints();


			if (viewSkel == true)
			{
				// Draw the bones
				DrawBodyBones(frameBodyJoints, 0);

				// Draw the Angles
				DrawBodyAngles(frameBodyJoints, side);

				// Draw the joints
				DrawBodyJoints(frameBodyJoints, 0);

				// Hides the face
				//HideFace(frameBodyJoints, 0);
			}
		}

		cv::imshow("GRIDDS: Body Viewer", frameDst);


		char key = (char)cv::waitKeyEx(33); // 33 ms corresponds to 30 fps

		if ((key == 's') && (g_appCurrentFrame < totalFrames))
		{
			g_appCurrentFrame++;
			frameChanged = 1;
			status = PlayMode::Pause;
		}

		if ((key == 'a') && (g_appCurrentFrame > 1))
		{
			g_appCurrentFrame--;
			frameChanged = 1;
			status = PlayMode::Pause;
		}

		if (key == 'v')
		{
			if (viewSkel == true)
				viewSkel = false;
			else
				viewSkel = true;
		}

		if (key == 'r')
		{
			status = PlayMode::Run;
		}

		if (key == 27) { break; }

		if ((status == PlayMode::Run) && (g_appCurrentFrame < totalFrames))
		{
			g_appCurrentFrame++;
			frameChanged = 1;
		}

		g_sequence.set(cv::CAP_PROP_POS_FRAMES, (g_appCurrentFrame - 1));


		if (frameChanged == 1)
		{
			cout << "Current Frame = " << g_appCurrentFrame << endl;
			frameChanged = 0;
		}

		g_sequence >> frame;



	}
	return(0);
}
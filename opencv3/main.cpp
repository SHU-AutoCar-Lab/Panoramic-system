//运行环境vs2013+opencv3.0
//上海大学  刘冬军   dongjliu@126.com
//输入：鱼眼摄像头  输出：俯视鸟瞰全景图像
//2016.11.14
#include  "opencv2/highgui.hpp"
#include  "opencv2/imgproc.hpp"
#include  <iostream>
#include  <stdio.h>
#include <ctime>
#include "opencv2/calib3d/calib3d.hpp"
using  namespace  cv;
using  namespace  std;

int  main(int argc, char**  argv)
{
	/*****************************************************************************/
	void front_remap(Mat &inimage,Mat &outimage);
	void left_remap(Mat &inimage,Mat &outimage);
	void back_remap(Mat &inimage,Mat &outimage);
	void right_remap(Mat &inimage,Mat &outimage);
	void left_rotate(Mat &inimage,Mat &outimage);
	void right_rotate(Mat &inimage,Mat &outimage);
	void back_rotate(Mat &inimage,Mat &outimage);
	void front_perspective(Mat &inimage, Mat &outimage);
	void left_perspective(Mat &inimage, Mat &outimage);
	void right_perspective(Mat &inimage, Mat &outimage);
	void back_perspective(Mat &inimage,Mat &outimage);
	void shift(Mat &inimage,Mat &outimage,int dx,int dy);
	Mat front,left,back,right;			//原图
	Mat front_t,left_t,back_t,right_t;	//校正图
	Mat left_r,back_r,right_r;		    //旋转变换后图
	Mat front_p,left_p,back_p,right_p;   //透视变换图 
	Mat model,output;
	VideoCapture capture0, capture1, capture2,capture3;
	clock_t start, stop;
	double time;


	/*/*************************************************读取摄像头*************************************************/
   	capture0.open(0);	
	capture1.open(1);
	capture2.open(2);	
	capture3.open(3);
	unsigned char cam0_en=0;
	unsigned char cam1_en=0;
	unsigned char cam2_en=0;
	unsigned char cam3_en=0;
    // Init camera
    if (!capture0.isOpened())
    {
        cout << "capture device 0 failed to open!" << endl;
    }
	else
		cam0_en=1;

	if (!capture1.isOpened())
    {
        cout << "capture device 1 failed to open!" << endl;
    }
  	else
		cam1_en=1;

	if (!capture2.isOpened())
    {
        cout << "capture device 2 failed to open!" << endl;
	}
	else
		cam2_en=1;

	if (!capture3.isOpened())
    {
        cout << "capture device 3 failed to open!" << endl;
    }
	else
		cam3_en=1;

	unsigned x=0;
	double w =640, h =360;
    capture2.set(CAP_PROP_FRAME_WIDTH,w);
    capture2.set(CAP_PROP_FRAME_HEIGHT,h);
    capture3.set(CAP_PROP_FRAME_WIDTH,w);
    capture3.set(CAP_PROP_FRAME_HEIGHT,h);
    capture1.set(CAP_PROP_FRAME_WIDTH,w);
    capture1.set(CAP_PROP_FRAME_HEIGHT,h);
    capture0.set(CAP_PROP_FRAME_WIDTH,w);
    capture0.set(CAP_PROP_FRAME_HEIGHT,h);
	
	model=imread("model.jpg");
	//Size msize = Size(152,262);
   // Mat right_k = Mat(msize,CV_32S);	// 右方图像放大
   // resize(model,model_1,msize);
	//imwrite("model.jpg",model_1);
	//waitKey();
    for(;;)
	{
		start = clock();
		cout<<"--------------------------------------"<<endl;
		if(cam0_en=1)
		 {
    		if(capture0.read(front))
			{
			cout<<"cam0:OK"<<endl;
			//imshow("0",front);
			//imwrite("0.jpg",frame0);
		}
		else
			cout<<"cam0:Fail"<<endl;
	}
	 if(cam1_en=1)
	 {
    		if(capture1.read(left))
		{
			cout<<"cam1:OK"<<endl;
			//imshow("1", left);
			//imwrite("1.jpg",left);
		}
		else
			cout<<"cam1:Fail"<<endl;
	 }

	if(cam2_en=1)
	 {
  		if(capture2.read(back))
		{
			cout<<"cam2:OK"<<endl;
			//imshow("2", back);
			//imwrite("2.jpg",back);
		}
		else
			cout<<"cam2:Fail"<<endl;
	 }

	if(cam3_en=1)
	{
  		if(capture3.read(right))
		{
			cout<<"cam3:OK"<<endl;
			//imshow("3", right);
			//imwrite("3.jpg",right);
		}
		else
			cout<<"cam3:Fail"<<endl;
	 }
		/*front=imread("cap0.jpg");
	//left=imread("cap1.jpg");
	//back=imread("cap2.jpg");
	//right=imread("cap3.jpg");*/
	front_remap(front,front_t);	//前方图像校正
	//imshow("front_t",front_t);
	//imwrite("front_t.jpg",front_t);
	//waitKey();
	left_remap(left,left_t);    //左方图像校正
	//imshow("left_t",left_t);
	//imwrite("left_t.jpg",left_t);
	//waitKey()；
	back_remap(back,back_t);	//后方图像校正
	//imshow("back_t",back_t);
	//imwrite("back_t.jpg",back_t);
	//waitKey();
	right_remap(right,right_t);	//右方图像校正
	//imshow("right_t",right_t);
	//imwrite("right_t.jpg",right_t);
	//waitKey();
	left_rotate(left_t,left_r);	//左方旋转变换
	//imwrite("left_r.jpg",left_r);
	//imshow("lr",left_r);
	//waitKey();
	right_rotate(right_t,right_r); //右方图像旋转
	//imwrite("right_r.jpg",right_r);
	//imshow("rr",right_r);
	//waitKey();
	back_rotate(back_t,back_r);      //后方图像旋转
	//imshow("back_r",back_r);
	//imwrite("back_r.jpg",back_r);
	//waitKey();
    front_perspective(front_t,front_p);	// 前方透视变换
	//imshow("front_p",front_p);
	//imwrite("front_p.jpg",front_p);
	//waitKey();
	left_perspective(left_r,left_p);	//左方透视变换
	//imshow("left_p",left_p);
	//imwrite("left_p.jpg",left_p);
	//waitKey();
	back_perspective(back_r,back_p);	 //后方图像透视变换
	//imshow("back_p",back_p);
	//imwrite("back_p.jpg",back_p);
	//waitKey();
	right_perspective(right_r,right_p);		//右侧透视变换
	//imshow("right_p",right_p);
	//imwrite("right_p.jpg",right_p);
	//waitKey();
	Size dsize = Size(640,640*1.06);
    Mat right_k = Mat(dsize,CV_32S);	// 右方图像放大
    resize(right_p,right_k,dsize);
	//imwrite("right_k.jpg",right_k);
	Mat src0, src1, src2, src3, src4,src5,src6, dst; 
	/*Mat right_ROI;
	Rect rect(40,40,1280,1280);  
	right_k(rect).copyTo(right_ROI);
	 
    src1 = imread("front_p.jpg");  
    src0 = imread("left_p.jpg"); 
	src3 = imread("right_p.jpg");
	src5 = imread("back_p.jpg");*/
	src1=front_p.clone();
	src0 = left_p.clone();
	src3 = right_k.clone();
	src5 = back_p.clone();
	shift(src0,src2,7,12);       //1=front 2=left  4=right  6=back
	shift(src3,src4,-2,-26);
	shift(src5,src6,14,-10);
	double alpha=0;
	double beta=1;
	double gama=0;
    dst.create(640,640 ,src1.type());  
	for(int i=0;i<dst.rows;i++)
	{
		uchar* dst_ptr  = dst.ptr<uchar>(i);
		for(int j=0;j<dst.cols*3;j++)
		{
			dst_ptr[j]=0;
		}
	}
    const int nChannels = src1.channels();  
   //前方与左侧//
	for (int i=0; i<350; i++)    //place front first
    {  
        const uchar* src1_ptr = src1.ptr<uchar>(i);   
        uchar* dst_ptr  = dst.ptr<uchar>(i);  
        for (int j=(400*nChannels);j<src1.cols*nChannels; j++)  
        { 
				dst_ptr[j] = src1_ptr[j];
        }  
    }  
	for (int i=350; i<src2.rows; i++)    //place left
    {  
        const uchar* src2_ptr = src2.ptr<uchar>(i);   
        uchar* dst_ptr  = dst.ptr<uchar>(i);  
        for (int j=0; j<src2.cols*nChannels; j++)  
        { 
				dst_ptr[j] = src2_ptr[j];
        }  
    }
	//imshow("kk",dst);
	//waitKey();
    for (int i=0; i<400; i++)  
    {  
        const uchar* src1_ptr = src1.ptr<uchar>(i);  
        const uchar* src2_ptr = src2.ptr<uchar>(i);  
        uchar* dst_ptr  = dst.ptr<uchar>(i);  
        for (int j=0; j<500; j++)  
        { 
				int courentcol=j*nChannels;
				if (src1_ptr[courentcol]==0 || src1_ptr[courentcol+1]==0 || src1_ptr[courentcol+2]==0)
				{
					dst_ptr[courentcol]=src2_ptr[courentcol];
					dst_ptr[courentcol+1]=src2_ptr[courentcol+1];
					dst_ptr[courentcol+2]=src2_ptr[courentcol+2];
				}
				else if (src2_ptr[courentcol]==0 || src2_ptr[courentcol+1]==0 || src2_ptr[courentcol+2]==0)
				{
					dst_ptr[courentcol]=src1_ptr[courentcol];
					dst_ptr[courentcol+1]=src1_ptr[courentcol+1];
					dst_ptr[courentcol+2]=src1_ptr[courentcol+2];
				}
				else
				{
					dst_ptr[courentcol] = src1_ptr[courentcol]*alpha + src2_ptr[courentcol]*beta + gama;
					dst_ptr[courentcol+1] = src1_ptr[courentcol+1]*alpha + src2_ptr[courentcol+1]*beta + gama;
					dst_ptr[courentcol+2] = src1_ptr[courentcol+2]*alpha + src2_ptr[courentcol+2]*beta + gama;
				}
        }  
    }
	//imshow("kk",dst);
	//waitKey();

    //左侧和后侧
    for (int i=440;i<640; i++)  
    {  
        const uchar* src2_ptr = src2.ptr<uchar>(i);  
        const uchar* src6_ptr = src6.ptr<uchar>(i);  
        uchar* dst_ptr  = dst.ptr<uchar>(i);  
        for (int j=0; j<220*nChannels; j++)  
        { 
				int courentcol=j*nChannels;
				if (src2_ptr[courentcol]==0 || src2_ptr[courentcol+1]==0 || src2_ptr[courentcol+2]==0)
				{
					dst_ptr[courentcol]=src6_ptr[courentcol];
					dst_ptr[courentcol+1]=src6_ptr[courentcol+1];
					dst_ptr[courentcol+2]=src6_ptr[courentcol+2];
				}
				else if (src6_ptr[courentcol]==0 || src6_ptr[courentcol+1]==0 || src6_ptr[courentcol+2]==0)
				{
					dst_ptr[courentcol]=src2_ptr[courentcol];
					dst_ptr[courentcol+1]=src2_ptr[courentcol+1];
					dst_ptr[courentcol+2]=src2_ptr[courentcol+2];
				}
				else
				{
					dst_ptr[courentcol] = src6_ptr[courentcol]*alpha + src2_ptr[courentcol]*beta + gama;
					dst_ptr[courentcol+1] = src6_ptr[courentcol+1]*alpha + src2_ptr[courentcol+1]*beta + gama;
					dst_ptr[courentcol+2] = src6_ptr[courentcol+2]*alpha + src2_ptr[courentcol+2]*beta + gama;
				} 
        }  
    }
	for (int i=440;i<src5.rows;i++)  
    {  
        const uchar* src6_ptr = src6.ptr<uchar>(i);   
        uchar* dst_ptr  = dst.ptr<uchar>(i);  
        for (int j=220*nChannels; j<src5.cols*nChannels; j++)  
        { 
				dst_ptr[j] = src6_ptr[j];
        }  
    }
	//imshow("kk",dst);
	//waitKey();

	///////前方和右侧
	for (int i=165; i<src4.rows; i++)  
    {  
        const uchar* src4_ptr = src4.ptr<uchar>(i);   
        uchar* dst_ptr  = dst.ptr<uchar>(i);  
        for (int j=410*nChannels; j<src4.cols*nChannels; j++)  
        { 
				dst_ptr[j] = src4_ptr[j];
        }  
    }
	 for (int i=0; i<180; i++)  
    {  
        const uchar* src1_ptr = src1.ptr<uchar>(i);  
        const uchar* src4_ptr = src4.ptr<uchar>(i);  
        uchar* dst_ptr  = dst.ptr<uchar>(i);  
        for (int j=410; j<src4.cols; j++)  
        { 
				int courentcol=j*nChannels;
				if (src1_ptr[courentcol]==0 || src1_ptr[courentcol+1]==0 || src1_ptr[courentcol+2]==0)
				{
					dst_ptr[courentcol]=src4_ptr[courentcol];
					dst_ptr[courentcol+1]=src4_ptr[courentcol+1];
					dst_ptr[courentcol+2]=src4_ptr[courentcol+2];
				}
				else if (src4_ptr[courentcol]==0 || src4_ptr[courentcol+1]==0 || src4_ptr[courentcol+2]==0)
				{
					dst_ptr[courentcol]=src1_ptr[courentcol];
					dst_ptr[courentcol+1]=src1_ptr[courentcol+1];
					dst_ptr[courentcol+2]=src1_ptr[courentcol+2];
				}
				else
				{
					dst_ptr[courentcol] = src1_ptr[courentcol]*alpha + src4_ptr[courentcol]*beta + gama;
					dst_ptr[courentcol+1] = src1_ptr[courentcol+1]*alpha + src4_ptr[courentcol+1]*beta + gama;
					dst_ptr[courentcol+2] = src1_ptr[courentcol+2]*alpha + src4_ptr[courentcol+2]*beta + gama;
				}
        }  
    }
	for (int i=440;i<src5.rows; i++)  
    {  
        const uchar* src4_ptr = src4.ptr<uchar>(i);  
        const uchar* src6_ptr = src6.ptr<uchar>(i);  
        uchar* dst_ptr  = dst.ptr<uchar>(i);  
        for (int j=300; j<src4.cols; j++)  
        { 
				int courentcol=j*nChannels;
				if (src4_ptr[courentcol]==0 || src4_ptr[courentcol+1]==0 || src4_ptr[courentcol+2]==0)
				{
					dst_ptr[courentcol]=src6_ptr[courentcol];
					dst_ptr[courentcol+1]=src6_ptr[courentcol+1];
					dst_ptr[courentcol+2]=src6_ptr[courentcol+2];
				}
				else if (src6_ptr[courentcol]==0 || src6_ptr[courentcol+1]==0 || src6_ptr[courentcol+2]==0)
				{
					dst_ptr[courentcol]=src4_ptr[courentcol];
					dst_ptr[courentcol+1]=src4_ptr[courentcol+1];
					dst_ptr[courentcol+2]=src4_ptr[courentcol+2];
				}
				else
				{
					dst_ptr[courentcol] = src4_ptr[courentcol]*alpha + src6_ptr[courentcol]*beta + gama;
					dst_ptr[courentcol+1] = src4_ptr[courentcol+1]*alpha + src6_ptr[courentcol+1]*beta + gama;
					dst_ptr[courentcol+2] = src4_ptr[courentcol+2]*alpha + src6_ptr[courentcol+2]*beta + gama;
				} 
        }  

    }

	for (int i=173; i<440; i++)  
    {  
        uchar* model_1 = model.ptr<uchar>(i-173);   
        uchar* dst_ptr  = dst.ptr<uchar>(i);  
        for (int j=265*nChannels; j<424*nChannels; j++)  
        { 
				dst_ptr[j] = model_1[j-265*nChannels];
        }  
    }
	dst(Rect(10, 10, 630, 590)).copyTo(output);
	Size ssize = Size(650,650);
    Mat result = Mat(dsize,CV_32S);	// 右方图像放大
    resize(output,result,dsize);
	//stop = clock(); 
	//time=(double)(stop - start) / CLOCKS_PER_SEC;
	//cout<<"时间"<<time<<endl;
	imshow("1",result);
	 /*stop = clock(); 
	 time=(double)(stop - start) / CLOCKS_PER_SEC;
	 cout<<"时间"<<time<<endl;*/
	//imwrite("dst.jpg",dst);
    waitKey(3);      
   }

}


//**********************************前左后右四个图像校正**********************************//
//****************************************************************************************//
void front_remap(Mat &inimage,Mat &outimage)
{
	Mat newCameraMatrix = Mat(3,3,CV_32FC1,Scalar::all(0));
	Size image_size =inimage.size();
	Mat R = Mat::eye(3,3,CV_32F);
	Mat mapx = Mat(image_size,CV_32FC1);
    Mat mapy = Mat(image_size,CV_32FC1);
	Matx33d intrinsic_matrix(228.7574365416764, 0, 279.8579998175736,0, 232.6699649630497, 232.7320582296409,0, 0, 1);
	Vec4d distortion_coeffs(-0.0874154, 0.0781037, -0.158698, 0.0927466);
	fisheye::initUndistortRectifyMap(intrinsic_matrix,distortion_coeffs,R,intrinsic_matrix,image_size,CV_32FC1,mapx,mapy);
	outimage=inimage.clone();
	remap(inimage,outimage,mapx, mapy, INTER_LINEAR);
}
void left_remap(Mat &inimage,Mat &outimage)
{
	Mat newCameraMatrix = Mat(3,3,CV_32FC1,Scalar::all(0));
	Size image_size =inimage.size();
	Mat R = Mat::eye(3,3,CV_32F);
	Mat mapx = Mat(image_size,CV_32FC1);
    Mat mapy = Mat(image_size,CV_32FC1);
	Matx33d intrinsic_matrix(231.9951620402049, 0, 300.836692561697,0, 234.3822822406889, 275.8653467251771,0, 0, 1);
	Vec4d distortion_coeffs(-0.0797442, 0.0959548, -0.161391, 0.0746264);
	fisheye::initUndistortRectifyMap(intrinsic_matrix,distortion_coeffs,R,intrinsic_matrix,image_size,CV_32FC1,mapx,mapy);
	outimage=inimage.clone();
	remap(inimage,outimage,mapx, mapy, INTER_LINEAR);
}
void back_remap(Mat &inimage,Mat &outimage)
{
	Mat newCameraMatrix = Mat(3,3,CV_32FC1,Scalar::all(0));
	Size image_size =inimage.size();
	Mat R = Mat::eye(3,3,CV_32F);
	Mat mapx = Mat(image_size,CV_32FC1);
    Mat mapy = Mat(image_size,CV_32FC1);
	Matx33d intrinsic_matrix(230.0115266369131, 0, 321.9881951948702,0, 232.5766417127015, 214.9958014488319,0, 0, 1);
	Vec4d distortion_coeffs(-0.106902, 0.112763, -0.127697, 0.0518379);
	fisheye::initUndistortRectifyMap(intrinsic_matrix,distortion_coeffs,R,intrinsic_matrix,image_size,CV_32FC1,mapx,mapy);
	outimage=inimage.clone();
	remap(inimage,outimage,mapx, mapy, INTER_LINEAR);
}
void right_remap(Mat &inimage,Mat &outimage)
{
	Mat newCameraMatrix = Mat(3,3,CV_32FC1,Scalar::all(0));
	Size image_size =inimage.size();
	Mat R = Mat::eye(3,3,CV_32F);
	Mat mapx = Mat(image_size,CV_32FC1);
    Mat mapy = Mat(image_size,CV_32FC1);
	Matx33d intrinsic_matrix(224.1073060628731, 0, 315.8351828442915,0, 227.9510986943261, 233.4523127884099,0, 0, 1);
	Vec4d distortion_coeffs(-0.040087, -0.0436213, 0.0303138, -0.00559813);
	fisheye::initUndistortRectifyMap(intrinsic_matrix,distortion_coeffs,R,intrinsic_matrix,image_size,CV_32FC1,mapx,mapy);
	outimage=inimage.clone();
	remap(inimage,outimage,mapx, mapy, INTER_LINEAR);
}


//**************************************************左侧右侧后侧图像旋转变换***************************************//
//****************************************************************************************************************//
void left_rotate(Mat &inimage,Mat &outimage)
{
	Mat temp;
	transpose(inimage,temp);
	flip(temp,outimage,0);
}
void right_rotate(Mat &inimage,Mat &outimage)
{
	Mat temp;
	transpose(inimage,temp);
	flip(temp,outimage,1);
}
void back_rotate(Mat &inimage,Mat &outimage)
{
	Mat temp;
	Mat turn_left;
	transpose(inimage,temp);
	flip(temp,turn_left,0);
	transpose(turn_left,temp);
	flip(temp,outimage,0);
}


//***************************************************前左后右透视变换成俯视图**********************************************//
//*************************************************************************************************************************//
void front_perspective(Mat &inimage, Mat &outimage)
{
	 Mat h( 3, 3, CV_32FC1 );
     vector<Point2f> pt1(4);
     vector<Point2f> pt2(4);
	 Mat warp_dst =Mat::zeros( inimage.rows, inimage.cols, inimage.type());
	 pt1[0] = Point2f(295,22) ;    
     pt1[1] = Point2f(392,22) ; 
     pt1[2] = Point2f(392,119) ; 
     pt1[3] = Point2f(295,119) ;  

     pt2[0] = Point2f(235,236); 
     pt2[1] = Point2f(340,236);                 
     pt2[2] = Point2f(370,345);                    
     pt2[3] = Point2f(210,346);
	 h=getPerspectiveTransform(pt2,pt1);
     warpPerspective(inimage,outimage,h,warp_dst.size());
}
void left_perspective(Mat &inimage, Mat &outimage)
{
	 Mat h( 3, 3, CV_32FC1 );
     vector<Point2f> pt1(4);
     vector<Point2f> pt2(4);
	 Mat warp_dst =Mat::zeros(640,640,inimage.type());
	 pt1[0] = Point2f(84,330) ;    
     pt1[1] = Point2f(84,233) ; 
     pt1[2] = Point2f(181,233) ; 
     pt1[3] = Point2f(181,330) ;  

     pt2[0] = Point2f(239,347); //496.0167 147.4822 
     pt2[1] = Point2f(240,248);                 
     pt2[2] = Point2f(330,213);                    
     pt2[3] = Point2f(328,354);
	 h=getPerspectiveTransform(pt2,pt1);
     warpPerspective(inimage,outimage,h,warp_dst.size());
}

void back_perspective(Mat &inimage,Mat &outimage)
{
	 Mat h( 3, 3, CV_32FC1 );
     vector<Point2f> pt1(4);
     vector<Point2f> pt2(4);
	 Mat warp_dst =Mat::zeros(640,640,inimage.type());
	 pt1[0] = Point2f(381,582) ;    
     pt1[1] = Point2f(284,582) ; 
     pt1[2] = Point2f(284,485) ; 
     pt1[3] = Point2f(381,485) ;  

     pt2[0] = Point2f(367,226); 
     pt2[1] = Point2f(248,228);                 
     pt2[2] = Point2f(215,100);                    
     pt2[3] = Point2f(398,98);
	 h=getPerspectiveTransform(pt2,pt1);
     warpPerspective(inimage,outimage,h,warp_dst.size());
}

void right_perspective(Mat &inimage, Mat &outimage)
{
	 Mat h( 3, 3, CV_32FC1 );
     vector<Point2f> pt1(4);
     vector<Point2f> pt2(4);
	 Mat warp_dst =Mat::zeros(640,640,inimage.type());
	 pt1[0] = Point2f(581,272) ;    
     pt1[1] = Point2f(581,368) ; 
     pt1[2] = Point2f(487,368) ; 
     pt1[3] = Point2f(487,272) ;  

     pt2[0] = Point2f(256,260); //496.0167 147.4822 
     pt2[1] = Point2f(257,364);                 
     pt2[2] = Point2f(155,385);                    
     pt2[3] = Point2f(157,230);
	 h=getPerspectiveTransform(pt2,pt1);
     warpPerspective(inimage,outimage,h,warp_dst.size());
}


//////***********************************************平移变换**************************************************//
//*************************************************************************************************************//
void shift(Mat &inimage,Mat &outimage,int dx,int dy)   //向左平移X为负，右x为正;  向下平移y为正，向上平移为负
{
	const int row=640;
	const int col=640;
	CV_Assert(inimage.depth()==CV_8U);
	outimage.create(row,col,inimage.type());
	Vec3b *p;
	for (int i = 0; i <row;i++)
    {
		p=outimage.ptr<Vec3b>(i);
        for (int j = 0; j < col;j++)
        {
            int x = j -dx;
            int y = i -dy;
            if (x >= 0 && y >= 0 && x < col && y < row) 
			{
				p[j]=inimage.ptr<Vec3b>(y)[x];
			}
        }
    }
}

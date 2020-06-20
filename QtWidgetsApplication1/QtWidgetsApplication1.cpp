#include "QtWidgetsApplication1.h"
#include "librealsense2/rs.hpp"
#include <opencv2/opencv.hpp>   // Include OpenCV API

using namespace cv;
using namespace std;
using namespace rs2;

Mat depth_img;
colorizer color_map;
pipeline pipe;
bool is_streaming = false;
bool previous_state = false;

QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	timer = new QTimer(this);
}

void QtWidgetsApplication1::on_pushButton_open_cam_clicked()
{
	is_streaming = true;

	if (is_streaming != previous_state) {

		if (is_streaming)
		{
			connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
			timer->start(20);
			pipe.start();
			ui.textEdit->setText("camera is open");
		}
		previous_state = is_streaming;
	}
}

void QtWidgetsApplication1::on_pushButton_close_cam_clicked()
{
	is_streaming = false;

	if (is_streaming != previous_state) {

		if (!is_streaming)
		{
			disconnect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
			pipe.stop();
			ui.textEdit->setText("camera is closed");

			Mat black_img = Mat::zeros(depth_img.size(), CV_8UC3);

			qt_image = QImage((const unsigned char*)(black_img.data), black_img.cols, black_img.rows, QImage::Format_RGB888);

			ui.camera->setPixmap(QPixmap::fromImage(qt_image));

			ui.camera->resize(ui.camera->pixmap()->size());

		}

		previous_state = is_streaming;
	}
}

void QtWidgetsApplication1::update_window()
{
		frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
		frame depth = data.get_depth_frame().apply_filter(color_map);

		// Query frame size (width and height)
		const int w = depth.as<video_frame>().get_width();
		const int h = depth.as<video_frame>().get_height();

		// Create OpenCV matrix of size (w,h) from the colorized depth data
		depth_img = Mat(Size(w, h), CV_8UC3, (void*)depth.get_data(), Mat::AUTO_STEP);

		qt_image = QImage((const unsigned char*)(depth_img.data), depth_img.cols, depth_img.rows, QImage::Format_RGB888);

		ui.camera->setPixmap(QPixmap::fromImage(qt_image));
}
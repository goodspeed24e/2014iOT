#include <deque>
#include <QApplication>
#include <QWidget>
#include <QResizeEvent>
#include <player/player.hpp>


class TestWidget : public QWidget
{
public:
	TestWidget()
	{
		is_init = false;
		setAttribute(Qt::WA_PaintOnScreen);
		setAttribute(Qt::WA_OpaquePaintEvent);
		setAttribute(Qt::WA_NoSystemBackground);
		setUpdatesEnabled(false);

	}
protected:
	void resizeEvent(QResizeEvent* e)
	{
		if(!player)
			return;

               instek::Player::PlayerRect temp_rect;
                temp_rect.left = 0;
                temp_rect.right = e->size().width();
                temp_rect.bottom = e->size().height();
                temp_rect.top = 0;
                player->Resize(temp_rect);
	}

	void paintEvent(QPaintEvent*)
	{

		if(is_init)
			return;
		is_init = true;
		printf("paubt evebt,\n");
		engine = new instek::XvPaintEngine(winId());

		instek::RenderRect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = 1024;
		rect.h = 768;

		instek::StreamingClient::SetThreadNum(4);

		instek::StreamingClient::StreamingInfo s_info;
		s_info.username = "admin";
		s_info.password = "id";
		s_info.remote_ip = "172.16.2.129";
		//s_info.camera_id = 33;
		s_info.camera_id = 1;


		//live streaming
		s_info.live.remote_port = "3514";
		s_info.streaming_mode = 0; // 0: live, 1: playback
		s_info.live.streaming_profile = 6; //best
		s_info.live.width = 0;
		s_info.live.height = 480;

		// playback
		s_info.playback.remote_port = "60006";
		s_info.playback.sec = 0;
		s_info.playback.data_interval = 0;
		s_info.playback.data_rate = 1; // value-> 0, 1,2,4,8
		s_info.playback.direction = 0;

		//instek::RenderRect r1;

		//printf("x: %d, y: %d\n", rect.x, rect.y);
		instek::Player::PlayerRect temp_rect;
		temp_rect.left = 0;
		temp_rect.top = 0;
		temp_rect.right = rect.w;
		temp_rect.bottom = rect.h;
		player.reset(new instek::Player(s_info, temp_rect, engine));
		player->SetAudio(true);
	}

private:
	instek::XvPaintEngine* engine;
	boost::shared_ptr<instek::Player> player;
	bool is_init;
};

using namespace std;
int main(int argc, char* argv[])
{
        QApplication a(argc, argv);
	TestWidget widget;
//	TestWidget widget2;

	widget.show();
//	widget2.show();
	widget.resize(1024, 768);
//	widget2.resize(512, 512);

	return a.exec();
}

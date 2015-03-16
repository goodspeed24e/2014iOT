#include <player/player.hpp>
#include <deque>

using namespace std;
int main()
{
	putenv((char*)"SDL_VIDEO_WINDOW_POS=20,20");
	instek::VideoRenderer::GlobalInit();

	int screen_w = 0;
	int screen_h = 0;
	instek::VideoRenderer::GetScreenSize(screen_w, screen_h);
	

	printf("scresn w: %d h: %d\n", screen_w, screen_h);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 320;
	rect.h = 240;

	int x_size = 1;
	int y_size = 1;

	
	instek::VideoRenderer::InitScreen(rect.w * x_size, rect.h * y_size);
	instek::StreamingClient::SetThreadNum(4);

	instek::StreamingClient::StreamingInfo s_info;
	s_info.username = "admin";
	s_info.password = "id";
	s_info.remote_ip = "172.16.2.166";//"vincent-ubuntu.local";//"172.16.2.130";
	//s_info.remote_ip = "172.16.0.96";
	//s_info.remote_ip = "172.16.3.50";
	//s_info.remote_ip = "172.16.0.29";
	s_info.camera_id = 1;


	//live streaming
	s_info.live.remote_port = "3514";
	//s_info.streaming_mode = 0;
	s_info.live.streaming_profile = 6; //best
	s_info.live.width = 0;
	s_info.live.height = 480;
	
	//playback streaming
	s_info.playback.remote_port = "60006";
	s_info.streaming_mode = 1;
	//s_info.playback.direction = 0; //forward: 0 , backward = 1;
	//s_info.playback.sec = ::time(NULL)- 60*30; //backward
	s_info.playback.direction = 1; //forward: 0 , backward = 1;
	s_info.playback.sec = ::time(NULL); //backward
	s_info.playback.data_interval = 0;
	s_info.playback.data_rate = 1; // value-> 0, 1,2,4,8,
	
	std::deque<boost::shared_ptr<instek::Player> > players;
	//setup thread pool.
	//it needs to be called before you create your streaming client.
	//StreamingClient::SetThreadNum(1);
	time_t d = ::time(NULL) - 60*30;
	//printf("-------------->%d\n", d); 
	SDL_Rect r1;
	for (int y = 0; y < y_size; ++y)
	{
		for (int x = 0; x < x_size; ++x)
		{
			//printf("x: %d, y: %d\n", rect.x, rect.y);
			instek::Player::PlayerRect temp_rect;
			temp_rect.left = rect.x;
			temp_rect.right = rect.x + rect.w;
			temp_rect.bottom = rect.y + rect.h;
			temp_rect.top = rect.y;
			boost::shared_ptr<instek::Player> player(new instek::Player(s_info, temp_rect));
			//player->Seek(d);
			//player->PlayMode(0, 1);
			//player->Start();
			players.push_back(player);
			r1 = rect;
			rect.x += rect.w;
		}
		rect.x = 0;
		rect.y += rect.h;
	}
	
	printf("type 'q' to quit.\n");
	char quit_char;
	do
	{
		scanf("%c",&quit_char);
	} while (quit_char != 'q');

	/*players.front()->Stop();
	players.front()->Seek(1269945977 + (86400*10));
	players.front()->PlayMode(0, 1);
	players.front()->Start();
	int direct;
	float speed;
	do
	{
		scanf("%d %f", &direct, &speed);
		printf("---->%d %f\n", direct, speed);
		if (direct != 0 && direct != 1)
			break;

		if (players.front()->PlayMode(direct, speed))
		{
			printf("restart===================================\n");
		        players.front()->Start();
		}
	} while (true);*/

	players.clear();
}

#include <ctime>
#include "Breakout.h"
#include "Player.h"

Breakout::Breakout()
	: wall_thickness_(DEFAULT_WALL_WIDTH), 
	paddle_width_(DEFAULT_PADDLE_WIDTH_RATIO)
{
	srand((int)time(NULL));

	paddle_x_ = START_POSITION;
    restart();
    
    state_buffer_.initialize(screenSize(), true);

    trainig_mode_ = false;
}

Breakout::~Breakout(){
}

void Breakout::normalize(F& x, F& y) {

    const F magnitude = std::sqrt(x*x + y*y);;

    if(magnitude != 0) {
        F s = 1/magnitude;
        x *= s;
        y *= s;
    }
}

void Breakout::restart() {
    ball_x_ = (F)rand() / (F)RAND_MAX;		// 가운데 
    ball_y_ = START_POSITION;		// 가운데

    ball_vel_x_ = (F)rand() / (F)RAND_MAX - DEFAULT_BALL_SPEED;	// 좌/우 이동 속도
    ball_vel_y_ = -DEFAULT_BALL_SPEED;							// 위로 쏘세요

    normalize(ball_vel_x_, ball_vel_y_);
}

const VectorND<float>& Breakout::getStateBuffer(){
	for (int j = 0; j < height_; j++){
        for (int i = 0; i < width_; i++) {
            float value = 0.0;

            if (front_buffer_[j + width_* i] == '*') 		value = 1.0;
            else if (front_buffer_[j + width_*i] == '-') 	value = 0.5;

            state_buffer_[i + width_*j] = value;
        }
    }
    return state_buffer_;
}

void Breakout::printStateBuffer(){
	// not implemented yet~~
}

void Breakout::movePaddle(DirType dir, F dx){

	switch(dir){
	case LEFT:
		dx *= -paddle_width_;
		break;
	case RIGHT:
		dx *= paddle_width_;
		break;
	case STAY:
	default:
		// do nothing
		return;
	}
	paddle_x_ += dx;

    if (paddle_x_ < 0.0f) 						paddle_x_ = 0.0f;
    else if (paddle_x_ > 1.0f - paddle_width_)	paddle_x_ = 1.0f - paddle_width_;
}

F Breakout::updateSatus(float dt){
	//bool isblocking = false;
    F gamestat = 0.0f; // normal

	// when the ball encounters both sides of wall,
    if (ball_x_ < wall_thickness_ && ball_vel_x_ < 0.0f) {
        ball_vel_x_ = -ball_vel_x_;
    }
    else if (ball_x_ > 1.0 - wall_thickness_ && ball_vel_x_ > 0.0f) {
        ball_vel_x_ = -ball_vel_x_;
    }

    if (ball_y_ < wall_thickness_ && ball_vel_y_ < 0.0f) {

        if (ball_x_ >= paddle_x_ && ball_x_ <= paddle_x_ + paddle_width_) {

            ball_vel_y_ = -ball_vel_y_;
            ball_vel_x_ += ((float)rand() / (float)RAND_MAX - DEFAULT_BALL_SPEED) * 0.2f;

            // for faster training
            if(trainig_mode_) {
            	restart(); 	
            }

            if( std::abs(ball_x_ - (paddle_x_ + (paddle_width_/2))) < 0.05f ){
                //std::cout << "hit center";
                gamestat = 2.0f;
            } else {
                gamestat = 1.0f;    
            }
            //reward = 1.0f; 
            //isblocking = true;
             //reward
        }
        else {
            restart();
            //reward = -1.0f;
            gamestat = -1.0f;
            return gamestat;
        }
    }

    if (ball_y_ > 1.0 - wall_thickness_ && ball_vel_y_ > 0.0) {
        ball_vel_y_ = -ball_vel_y_;
    }

    ball_x_ += ball_vel_x_ * dt;
    ball_y_ += ball_vel_y_ * dt;

    return gamestat;
}

void Breakout::makeScene(){

	char paddleImg[] = "------";
	//char paddleImg_t[] = "-----";
    char* paddleImagePtr = nullptr;
    int paddleImgSize;

    //if(0/*trainig_mode_*/){
    //    paddleImgSize = sizeof(paddleImg_t) - 1;
    //    paddleImagePtr = paddleImg_t;
    //}
    //else{
        paddleImgSize = sizeof(paddleImg) - 1;
        paddleImagePtr = paddleImg;
    //}

	const char ballImg[] = "*";
	const int paddle_i = MIN2(
							MAX2(
								round(paddle_x_ * (float)width_ ), 
								0
							), 
							width_ - 1 - paddleImgSize 
						);
	const int ball_i = MIN2( 
							MAX2( 
								round(ball_x_ * (float)width_ - 1), 
								0
							), 
							width_ - 1
						);
    const int ball_j = MIN2( 
	    					MAX2(
	    						round(ball_y_ * (float)height_ - 1), 
	    						0
	    					), 
	    					height_ - 1
	    				);

    drawToBackBuffer(paddle_i, 0, (char*)paddleImagePtr);
    drawToBackBuffer(ball_i, ball_j, (char*)ballImg);
}


// end of file 

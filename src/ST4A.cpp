#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "can_plugins/Frame.h"
#include "can_utils.hpp"
#include<math.h>

sensor_msgs::Joy gmsg;

void joyCallback(const sensor_msgs::Joy::ConstPtr& cmsg)
{
    boost::shared_ptr<sensor_msgs::Joy> joy_ptr = boost::const_pointer_cast<sensor_msgs::Joy>(cmsg);

    gmsg = *joy_ptr;
}

int main(int argc, char **argv){

    ros::init(argc, argv, "SLC4");
  
    ros::NodeHandle n;

    ros::Publisher chatter = n.advertise<can_plugins::Frame>("can_tx",1000);

    ros::Subscriber sub = n.subscribe("joy", 1000, joyCallback);

    ros::Rate loop_rate(10);

    int count = 0;
    while (ros::ok()){
        const sensor_msgs::Joy& msg = gmsg;

        if(msg.buttons[2]==1)
        {
            chatter.publish(get_frame(0x300,static_cast<uint8_t>(4)));
        }
        //↑Bボタンでmode_posへ移行
        
        if(msg.buttons[1]==1)
        {
            chatter.publish(get_frame(0x300,static_cast<uint8_t>(1)));
        }
        //↑Aボタンで手動でmodeをdisableへ
        int pi_count = 0;
        if(msg.buttons[4]==1)
        {
            pi_count--;
        }
        //ZLボタンで減少
        if(msg.buttons[5])
        {
            pi_count++;
        }
        //ZRボタンで増加
        if(msg.buttons[3]==1)
        {
            pi_count = 0;
        }
        //Yボタンでリセット
        if(msg.buttons[0]==1)
        {
            chatter.publish(get_frame(0x301, 3.14f*(pi_count)));
        }
        //Xボタンで実行
        //can_plugins::Frame a = get_frame(0x101, 1.0f);

        ros::spinOnce();

        loop_rate.sleep();
        ++count;
    }

    return 0;
}
#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "can_plugins/Frame.h"
#include "can_utils.hpp"
#include<math.h>

//初回必ず何か操作しないといけない欠陥仕様

sensor_msgs::Joy gmsg;

int gcount = 0;

void joyCallback(const sensor_msgs::Joy::ConstPtr& cmsg)
{
    boost::shared_ptr<sensor_msgs::Joy> joy_ptr = boost::const_pointer_cast<sensor_msgs::Joy>(cmsg);

    gmsg = *joy_ptr;

    if(gcount == 0)
    {
        gcount++;
    }
}

int main(int argc, char **argv){

    ros::init(argc, argv, "SLC4");
  
    ros::NodeHandle n;
    ros::Publisher chatter = n.advertise<can_plugins::Frame>("can_tx",1000);

    ros::Subscriber sub = n.subscribe("joy", 1000, joyCallback);

    ros::Rate loop_rate(10);

    int count = 0;
    int pi_count = 0;
    while (ros::ok()){
        ROS_INFO("Tomo no sonshitu: 30000yen otu");
        if(gcount == 1)
        {
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
            if(msg.buttons[4]==1)
            {
                pi_count--;
            }
            //ZLボタンで減少
            if(msg.buttons[5]==1)
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
                ROS_INFO("chat");
            }
            //Xボタンで実行
            //can_plugins::Frame a = get_frame(0x101, 1.0f);
            ROS_INFO("%d",pi_count);
        }

        ros::spinOnce();

        loop_rate.sleep();
        ++count;
    }
    return 0;
}
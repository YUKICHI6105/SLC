#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "can_plugins/Frame.h"
#include "can_utils.hpp"
#include<math.h>

sensor_msgs::Joy::ConstPtr gmsg;

void joyCallback(const sensor_msgs::Joy::ConstPtr& cmsg)
{
    sensor_msgs::Joy::ConstPtr gmsg = cmsg;
}

int main(int argc, char **argv){

    ros::init(argc, argv, "SLC4");
  
    ros::NodeHandle n;

    ros::Publisher chatter = n.advertise<can_plugins::Frame>("can_tx",1000);

    ros::Subscriber sub = n.subscribe("joy", 1000, joyCallback);

    ros::Rate loop_rate(10);

    int count = 0;
    while (ros::ok()){
        const sensor_msgs::Joy::ConstPtr& msg = gmsg;

        if(msg->buttons[1]==1)
        {
            chatter.publish(get_frame(0x300,static_cast<uint8_t>(5)));
            chatter.publish(get_frame(0x110,static_cast<uint8_t>(5)));
            chatter.publish(get_frame(0x120,static_cast<uint8_t>(5)));
            chatter.publish(get_frame(0x130,static_cast<uint8_t>(5)));
        }
        //↑mode_velへ移行
        
        if(msg->buttons[3]==1)
        {
            chatter.publish(get_frame(0x300,static_cast<uint8_t>(0)));
            chatter.publish(get_frame(0x110,static_cast<uint8_t>(0)));
            chatter.publish(get_frame(0x120,static_cast<uint8_t>(0)));
            chatter.publish(get_frame(0x130,static_cast<uint8_t>(0)));
        }
        //↑手動でmodeをfolseへ

        //can_plugins::Frame a = get_frame(0x101, 1.0f);
        float x= -(msg->axes[0]);
        float y=  (msg->axes[1]);
        float r= 0;
        
        if(msg->buttons[4]==1)
        {
            r =1.0f;
        }
        else if(msg->buttons[4]==0)
        {
            r =0.0f;
        }
        //↑左回転
        if(msg->buttons[5]==1)
        {
            r =-1.0f;
        }
        else if(msg->buttons[5]==0)
        {
            r =0.0f;
        }
        //右回転
        chatter.publish(get_frame(0x301, y-x+r/2));
        chatter.publish(get_frame(0x111, x+y+r/2));
        chatter.publish(get_frame(0x121, x-y+r/2));
        chatter.publish(get_frame(0x131, -x-y+r/2));
        //chatter.publish(get_frame(0x101, x/static_cast<float>(sqrt(2))-y/static_cast<float>(sqrt(2))));
        //100右上、110左上、120左下、130右下

        ros::spinOnce();

        loop_rate.sleep();
        ++count;
    }

    return 0;
}
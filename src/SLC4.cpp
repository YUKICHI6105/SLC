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
            chatter.publish(get_frame(0x100,static_cast<uint8_t>(5)));
            chatter.publish(get_frame(0x110,static_cast<uint8_t>(5)));
            chatter.publish(get_frame(0x120,static_cast<uint8_t>(5)));
            chatter.publish(get_frame(0x130,static_cast<uint8_t>(5)));
        }
        //↑mode_velへ移行
        
        if(msg.buttons[1]==1)
        {
            chatter.publish(get_frame(0x100,static_cast<uint8_t>(1)));
            chatter.publish(get_frame(0x110,static_cast<uint8_t>(1)));
            chatter.publish(get_frame(0x120,static_cast<uint8_t>(1)));
            chatter.publish(get_frame(0x130,static_cast<uint8_t>(1)));
        }
        //↑手動でmodeをfalseへ

        //can_plugins::Frame a = get_frame(0x101, 1.0f);
        float x= -(msg.axes[0]);
        float y=  (msg.axes[1]);
        float r= 0;
        
        if(msg.buttons[4]==1)
        {
            r =1.0f;
        }
        //↑左回転
        else if(msg.buttons[5]==1)
        {
            r =-1.0f;
        }
        //↑右回転
        else if(msg.buttons[4]==msg.buttons[5])
        {
            r =0.0f;
        }
        chatter.publish(get_frame(0x101, 6.28f*(y-x+r)));
        chatter.publish(get_frame(0x111, 6.28f*(x+y+r)));
        chatter.publish(get_frame(0x121, 6.28f*(x-y+r)));
        chatter.publish(get_frame(0x131, 6.28f*(-x-y+r)));
        //chatter.publish(get_frame(0x101, x/static_cast<float>(sqrt(2))-y/static_cast<float>(sqrt(2))));
        //100右上、110左上、120左下、130右下

        ros::spinOnce();

        loop_rate.sleep();
        ++count;
    }

    return 0;
}
/* _________________________________
   |                                 |
   |           INCLUDES              |
   |_________________________________| */
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <rwsfi2016_libs/player.h>
#include <rwsfi2016_msgs/GameQuery.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl_ros/transforms.h>
#include <pcl.h>
#include <pcl/point_types.h>
#include <pcl_ros/point_cloud.h>

/* _________________________________
   |                                 |
   |              CODE               |
   |_________________________________| */
using namespace std;
using namespace ros;



/**
 * @brief MyPlayer extends class Player, i.e., there are additional things I can do with MyPlayer and not with any Player, e.g., to order a movement.
 */
class MyPlayer: public rwsfi2016_libs::Player
{
public:
    /**
     * @brief Constructor, nothing to be done here
     * @param name player name
     * @param pet_name pet name
     */

    typedef pcl::PointXYZRGB PointT;
    sensor_msgs::PointCloud2 pcll;

    bool respo(rwsfi2016_msgs::GameQuery::Request  &req,
               rwsfi2016_msgs::GameQuery::Response &res)
    {
        pcl::PointCloud<PointT> cloud;
        pcl::fromROSMsg(pcll, cloud);
        res.resposta = "Banana";
        return true;
    }

    void callback(const sensor_msgs::PointCloud2 msg)
    {
        pcll = msg;
    }

    ros::Publisher publisher;
    ros::ServiceServer servico;
    visualization_msgs::Marker bocas_msg;
    ros::Subscriber sub;




    MyPlayer(string player_name, string pet_name="/dog"): Player(player_name, pet_name){
        publisher = node.advertise<visualization_msgs::Marker>("/bocas", 1);
        sub = node.subscribe<sensor_msgs::PointCloud2> ("/object_point_cloud", 1, &MyPlayer::callback, this);
        bocas_msg.header.frame_id = name;
        bocas_msg.ns = name;
        bocas_msg.id = 0;
        bocas_msg.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
        bocas_msg.action = visualization_msgs::Marker::ADD;
        bocas_msg.scale.z = 0.4;
        bocas_msg.pose.position.y = 0.3;
        bocas_msg.color.a = 1.0; // Don't forget to set the alpha!
        bocas_msg.color.r = 0.0;
        bocas_msg.color.g = 0.0;
        bocas_msg.color.b = 0.0;

        servico = node.advertiseService(name + "/game_query", &MyPlayer::respo, this);
    };

    void play(const rwsfi2016_msgs::MakeAPlay& msg)
    {
        bocas_msg.header.stamp = ros::Time();
        // Player to kill
        int killkill = 1;
        // Distance to arena
        double distance_to_arena = getDistanceToArena();
        if (distance_to_arena > 7.5) { //behaviour move to the center of arena
            string arena = "/map";
            move(msg.max_displacement, getAngleToPLayer(arena));
            bocas_msg.text = "Nao vas para ai pah!!!";
        } else
        {
            if (msg.blue_alive.size() > 0) { // Se existir algum vivo
                // Kill player id "killkill"
                string kill_player_name = msg.blue_alive.at(killkill);
                for (int pl=0; pl<msg.blue_alive.size(); pl++) {
                    if (kill_player_name.compare(msg.blue_alive.at(pl)) != 0) {
                        // Procura o alive mais perto
                        double dist_min = 100000;
                        int angleMin = 0;
                        double dist = 0;
                        for (int pl=0; pl < msg.blue_alive.size(); pl++) {
                            dist = getDistanceToPlayer(msg.blue_alive.at(pl));
                            if ((dist < dist_min) && (!isnan(dist) ) ) {
                                killkill = pl;
                                dist_min = dist;
                            }
                        }
                    }
                    break;
                }
                move(msg.max_displacement, getAngleToPLayer(msg.blue_alive.at(killkill)));
                bocas_msg.text = msg.blue_alive.at(killkill) + ", toma toma toma foguetinhos!!!";
            } else { // Se estiverem todos mortos
                double dist_min_hunter = 100000;
                double dist_hunter = 0;
                int angleMinHunter = 0;
                for (int pl=0; pl < hunters_team->players.size(); pl++) {
                    dist_hunter = getDistanceToPlayer(hunters_team->players[pl]);
                    if ((dist_hunter < dist_min_hunter) && (!isnan(dist_hunter))) {
                        angleMinHunter = pl;
                        dist_min_hunter = dist_hunter;
                    }
                }
                // Foge do que estiver mais perto
                ROS_INFO_STREAM("Hunter mais proximo: " << hunters_team->players[angleMinHunter] << " angle: " << getAngleToPLayer(hunters_team->players[angleMinHunter]));
                double angle_temp = getAngleToPLayer(hunters_team->players[angleMinHunter]);
                double finalAngle = angle_temp+M_PI;
                if (angle_temp > 0)
                    finalAngle = angle_temp-M_PI;
                //MOVE//
                move(msg.max_displacement, finalAngle);
                bocas_msg.text = "Foge que vem ai o predador " + hunters_team->players[angleMinHunter];
            }
        }
        publisher.publish(bocas_msg);

    }


    //    void play(const rwsfi2016_msgs::MakeAPlay& msg)
    //        {
    //          //Custom play behaviour. Now I will win the game
    //            double distance_to_arena = getDistanceToArena();
    //            ROS_INFO("distance_to_arena = %f", distance_to_arena);

    //            double near_player_distance = 1000.0;
    //            int index_near_player = 0;

    //            for(int i=0; i < msg.blue_alive.size(); i++)
    //            {
    //               // std::cout << preys_team->players[i] << std::endl;
    //                float distance = getDistanceToPlayer(msg.blue_alive[i]);
    //                //std::cout << "Get Distance: "<< isnan(distance) << std::endl;

    //                if( (distance < near_player_distance) && (!isnan(distance)))
    //                {
    //                    near_player_distance = distance;
    //                    index_near_player    = i;
    //                }
    //            }

    //            double dist_min_hunter = 1000.0;
    //            double dist_hunter = 0.0;
    //            int angleMinHunter = 0;

    //            for (int pl=0; pl < hunters_team->players.size(); pl++)
    //            {
    //                dist_hunter = getDistanceToPlayer(hunters_team->players[pl]);

    //                if ((dist_hunter < dist_min_hunter) && (!isnan(dist_hunter)))
    //                {
    //                    angleMinHunter = pl;
    //                    dist_min_hunter = dist_hunter;
    //                }
    //            }

    //            float finalAngle;

    //            if (distance_to_arena > 7.3) //behaviour move to the center of arena
    //            {
    //                string arena = "/map";
    //                move(msg.max_displacement, getAngleToPLayer(arena));
    //                bocas_msg.text = "Quase!";
    //            }
    //            else if(dist_min_hunter < near_player_distance) //FUGIR
    //            {
    //                double angle_temp = getAngleToPLayer(msg.red_alive[angleMinHunter]); //caçador
    //                double angle_presa = getAngleToPLayer(msg.blue_alive[index_near_player]); //presa

    //                finalAngle = (angle_presa+angle_temp)/2.0;


    //                if (angle_temp > 0)
    //                    finalAngle = angle_temp-M_PI;

    //                move(msg.max_displacement,  finalAngle);
    //                bocas_msg.text = "Deixem-me jogar!";
    //            }
    //            else //CAÇAR
    //            {
    //                move(msg.max_displacement, getAngleToPLayer(msg.blue_alive[index_near_player]) );
    //                bocas_msg.text = "Vou-te apanhar!";
    //            }

    //            publisher.publish(bocas_msg);

    //          //Behaviour follow the closest prey
    //          //move(msg.max_displacement, getAngleToPLayer(msg.blue_alive[index_near_player]) );
    //        }
};


/**
 * @brief The main function. All you need to do here is enter your name and your pets name
 * @param argc number of command line arguments
 * @param argv values of command line arguments
 * @return result
 */
int main(int argc, char** argv)
{
    // ------------------------
    //Replace this with your name
    // ------------------------
    string my_name = "jleao";
    string my_pet = "/dog";

    //initialize ROS stuff
    ros::init(argc, argv, my_name);

    //Creating an instance of class MyPlayer
    MyPlayer my_player(my_name, my_pet);

    //Infinite spinning (until ctrl-c)
    ros::spin();
}

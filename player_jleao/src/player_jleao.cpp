/* _________________________________
   |                                 |
   |           INCLUDES              |
   |_________________________________| */
#include <ros/ros.h>
#include <rwsfi2016_libs/player.h>

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
    MyPlayer(string player_name, string pet_name="/dog"): Player(player_name, pet_name){};

    void play(const rwsfi2016_msgs::MakeAPlay& msg)
    {
       /* double near_player_distance = 1000.0;
        int index_near_player = 0;

        std::cout << "Number of Preys: "<< preys_team->players.size() << std::endl;
        std::cout << "Jogadores Vivos"<< msg.blue_alive[0] << std::endl;
        if (msg.blue_alive.size() == 0)
        {
            ROS_INFO_STREAM("Hunter mais proximo: " << hunters_team->players[angleMinHunter] << " angle: " << getAngleToPLayer(hunters_team->players[angleMin]));
            double angle_temp = getAngleToPLayer(hunters_team->players[angleMinHunter]);
            finalAngle = angle_temp+M_PI;
            if (angle_temp > 0)
                finalAngle = angle_temp-M_PI;
            move(msg.max_displacement, finalAngle);
        }
        else
        {
            for(int i=0; i < preys_team->players.size(); i++)
            {
                // std::cout << preys_team->players[i] << std::endl;
                float distance = getDistanceToPlayer(preys_team->players[i]);
                //std::cout << "Get Distance: "<< isnan(distance) << std::endl;

                if( (distance < near_player_distance) && (!isnan(distance)))
                {
                    near_player_distance = distance;
                    index_near_player    = i;
                }
            }
        }
        //Behaviour follow the closest prey
        move(msg.max_displacement, getAngleToPLayer(preys_team->players[index_near_player]) );
    }*/




    //##########################

    double distance_to_arena = getDistanceToArena();
            ROS_INFO("distance_to_arena = %f", distance_to_arena);

            if (distance_to_arena > 6) //behaviour move to the center of arena
            {
                string arena = "/map";
                move(msg.max_displacement, getAngleToPLayer(arena));
            }


            double near_player_distance = 1000.0;
            int index_near_player = 0;

            std::cout << "Number of Preys: "<< preys_team->players.size() << std::endl;

            for(int i=0; i < msg.blue_alive.size(); i++)
            {
               // std::cout << preys_team->players[i] << std::endl;
                float distance = getDistanceToPlayer(msg.blue_alive[i]);
                //std::cout << "Get Distance: "<< isnan(distance) << std::endl;

                if( (distance < near_player_distance) && (!isnan(distance)))
                {
                    near_player_distance = distance;
                    index_near_player    = i;
                }
            }

          //Behaviour follow the closest prey
          move(msg.max_displacement, getAngleToPLayer(msg.blue_alive[index_near_player]) );


    //Caça e Foge
    /*void play(const rwsfi2016_msgs::MakeAPlay& msg)
        {
            //Behaviour follow the closest prey
            double dist_min = 100000;
            int angleMin = 0;
            double dist = 0;
            for (int pl=0; pl < preys_team->players.size(); pl++) {
                dist = getDistanceToPlayer(preys_team->players[pl]);
                if ((dist < dist_min) && (!isnan(dist) ) ) {
                    angleMin = pl;
                    dist_min = dist;
                }
            }
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
            // Find team mates
            double dist_min_team = 100000;
            double dist_team = 0;
            int angleMinteam = 0;
            for (int pl=0; pl < my_team->players.size(); pl++) {
                dist_team = getDistanceToPlayer(my_team->players[pl]);
                if ((dist_team < dist_min_team) && (!isnan(dist_team))) {
                    angleMinteam = pl;
                    dist_min_team = dist_team;
                }
            }

            double finalAngle = 0.0;
            if (dist_min_hunter < dist_min) {
                if (dist_min_hunter < dist_min_team) {
                    ROS_INFO_STREAM("Hunter mais proximo: " << hunters_team->players[angleMinHunter] << " angle: " << getAngleToPLayer(hunters_team->players[angleMin]));
                    double angle_temp = getAngleToPLayer(hunters_team->players[angleMinHunter]);
                    finalAngle = angle_temp+M_PI;
                    if (angle_temp > 0)
                        finalAngle = angle_temp-M_PI;
                    move(msg.max_displacement, finalAngle);
                } else {
                    ROS_INFO_STREAM("Team mais proximo: " << my_team->players[angleMinHunter] << " angle: " << getAngleToPLayer(my_team->players[angleMin]));
                    double angle_temp = getAngleToPLayer(my_team->players[angleMinteam]);
                    finalAngle = angle_temp+M_PI;
                    if (angle_temp > 0)
                        finalAngle = angle_temp-M_PI;
                    move(msg.max_displacement, finalAngle);
                }
            } else {
                ROS_INFO_STREAM("Preyer mais proximo: " << preys_team->players[angleMin] << " angle: " << getAngleToPLayer(preys_team->players[angleMin]));
                move(msg.max_displacement, getAngleToPLayer(preys_team->players[angleMin]));
            }
        }*/
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

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
        //Custom play behaviour. Now I will win the game
        /*float ang_min = 9999;
        int player_num = 0;

        for(int i = 0; i < preys_team->players.size(); i++)
        {
                ang_min = getAngleToPLayer(preys_team->players[i]);
                player_num = i;
        }
        move(msg.max_displacement, getAngleToPLayer(preys_team->players[player_num]));*/
        double near_player_distance = 1000.0;
                int index_near_player = 0;

                std::cout << "Number of Preys: "<< preys_team->players.size() << std::endl;

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

              //Behaviour follow the closest prey
              move(msg.max_displacement, getAngleToPLayer(preys_team->players[index_near_player]) );
    }
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
